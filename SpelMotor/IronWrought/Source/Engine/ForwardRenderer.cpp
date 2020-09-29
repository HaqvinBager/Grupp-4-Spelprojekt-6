#include "stdafx.h"
#include "ForwardRenderer.h"
#include "Engine.h"
#include "ModelInstance.h"
#include "Model.h"
#include "Camera.h"
#include "EnvironmentLight.h"
#include "PointLight.h"
#include "ModelMath.h"

namespace SM = DirectX::SimpleMath;

CForwardRenderer::CForwardRenderer() : myContext(nullptr), myFrameBuffer(nullptr), myFrameBufferData(), myObjectBuffer(nullptr), myObjectBufferData(), myBoneBuffer(nullptr), myBoneBufferData() {

}

CForwardRenderer::~CForwardRenderer() {
	myContext = nullptr;
}

bool CForwardRenderer::Init(CEngine& anEngine) {
	if (!anEngine.myFramework)
		return false;

	myContext = anEngine.myFramework->GetContext();
	if (!myContext) {
		return false;
	}

	ID3D11Device* device = anEngine.myFramework->GetDevice();
	if (!device)
		return false;

	HRESULT result;

	D3D11_BUFFER_DESC bufferDescription = { 0 };
	bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDescription.MiscFlags = 0;
	bufferDescription.ByteWidth = static_cast<UINT>(sizeof(SFrameBufferData) + (16 - (sizeof(SFrameBufferData) % 16)));
	bufferDescription.StructureByteStride = 0;

	result = device->CreateBuffer(&bufferDescription, nullptr, &myFrameBuffer);
	if (FAILED(result)) {
		return false;
	}

	bufferDescription.ByteWidth = static_cast<UINT>(sizeof(SObjectBufferData) + (16 - (sizeof(SObjectBufferData) % 16)));
	result = device->CreateBuffer(&bufferDescription, nullptr, &myObjectBuffer);
	if (FAILED(result)) {
		return false;
	}

	bufferDescription.ByteWidth = static_cast<UINT>(sizeof(SBoneBufferData) + (16 - (sizeof(SBoneBufferData) % 16)));
	result = device->CreateBuffer(&bufferDescription, nullptr, &myBoneBuffer);
	if (FAILED(result)) {
		return false;
	}

	return true;
}

void CForwardRenderer::Render(CEnvironmentLight* anEnvironmentLight, std::vector<std::pair<unsigned int, std::array<CPointLight*, 8>>> somePointLights, CCamera* aCamera, std::vector<CModelInstance*>& aModelList) {
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE bufferData;
	myFrameBufferData.myToCamera = aCamera->GetTransform().Invert();
	myFrameBufferData.myToProjection = aCamera->GetProjection();
	myFrameBufferData.myCameraPosition = aCamera->GetPosition();
	myFrameBufferData.myDirectionalLightDirection = anEnvironmentLight->GetDirection();
	myFrameBufferData.myDirectionalLightColor = anEnvironmentLight->GetColor();

	ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));

	result = myContext->Map(myFrameBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
	if (FAILED(result)) {
		return;
	}
	memcpy(bufferData.pData, &myFrameBufferData, sizeof(SFrameBufferData));
	myContext->Unmap(myFrameBuffer, 0);
	myContext->VSSetConstantBuffers(0, 1, &myFrameBuffer);
	myContext->PSSetConstantBuffers(0, 1, &myFrameBuffer);

	ID3D11ShaderResourceView* cubeShaderResourceView = anEnvironmentLight->GetCubeMap();
	myContext->PSSetShaderResources(0, 1, &cubeShaderResourceView);
	
	UINT modelInstanceCount = 0;
	for (CModelInstance* instance : aModelList) {
		CModel* model = instance->GetModel();
		CModel::SModelData modelData = model->GetModelData();

		myObjectBufferData.myToWorld = instance->GetTransform();
		UINT numberOfUsedPointLights = somePointLights[modelInstanceCount].first;
		for (UINT i = 0; i < numberOfUsedPointLights && i < 8; ++i)
		{
			myObjectBufferData.myPointLights[i].myPosition = somePointLights[modelInstanceCount].second[i]->GetPosition();
			myObjectBufferData.myPointLights[i].myRange = somePointLights[modelInstanceCount].second[i]->GetRange();
			
			SM::Vector3 color = somePointLights[modelInstanceCount].second[i]->GetColor();
			float intensity = somePointLights[modelInstanceCount].second[i]->GetIntensity();
			myObjectBufferData.myPointLights[i].myColorAndIntensity = SM::Vector4(color.x, color.y, color.z, intensity);
		}
		myObjectBufferData.myNumberOfUsedPointLights = 1;
		modelInstanceCount++;

		D3D11_MAPPED_SUBRESOURCE PSbufferData;
		ZeroMemory(&PSbufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
		result = myContext->Map(myObjectBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &PSbufferData);
		if (FAILED(result)) {
			return;
		}
		CopyMemory(PSbufferData.pData, &myObjectBufferData, sizeof(SObjectBufferData));
		myContext->Unmap(myObjectBuffer, 0);

		memcpy(myBoneBufferData.myBones, instance->GetBones().data(), sizeof(SlimMatrix44) * 64);

		D3D11_MAPPED_SUBRESOURCE VSBufferData;
		ZeroMemory(&VSBufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
		result = myContext->Map(myBoneBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &VSBufferData);
		if (FAILED(result)) {
			return;
		}
		CopyMemory(VSBufferData.pData, &myBoneBufferData, sizeof(SBoneBufferData));
		myContext->Unmap(myBoneBuffer, 0);


		myContext->IASetPrimitiveTopology(modelData.myPrimitiveTopology);
		myContext->IASetInputLayout(modelData.myInputLayout);
		myContext->IASetVertexBuffers(0, 1, &modelData.myVertexBuffer, &modelData.myStride, &modelData.myOffset);
		myContext->IASetIndexBuffer(modelData.myIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		myContext->VSSetConstantBuffers(1, 1, &myObjectBuffer);
		myContext->VSSetConstantBuffers(2, 1, &myBoneBuffer);
		myContext->VSSetShader(modelData.myVertexShader, nullptr, 0);

		myContext->PSSetConstantBuffers(1, 1, &myObjectBuffer);
		myContext->PSSetShaderResources(1, 3, &modelData.myTexture[0]);
		myContext->PSSetSamplers(0, 1, &modelData.mySamplerState);
		myContext->PSSetShader(modelData.myPixelShader, nullptr, 0);

		myContext->DrawIndexed(modelData.myNumberOfIndicies, 0, 0);
	}
}