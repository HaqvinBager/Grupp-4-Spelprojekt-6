#include "stdafx.h"
#include "ForwardRenderer.h"
#include "Engine.h"
#include "ModelInstance.h"
#include "Model.h"
#include "Camera.h"
#include "EnvironmentLight.h"


CForwardRenderer::CForwardRenderer() {
	myContext = nullptr;
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
	bufferDescription.ByteWidth = sizeof(SFrameBufferData);

	result = device->CreateBuffer(&bufferDescription, nullptr, &myFrameBuffer);
	if (FAILED(result)) {
		return false;
	}

	bufferDescription.ByteWidth = sizeof(SObjectBufferData);
	result = device->CreateBuffer(&bufferDescription, nullptr, &myObjectBuffer);
	if (FAILED(result)) {
		return false;
	}

	return true;
}

void CForwardRenderer::Render(CEnvironmentLight* anEnvironmentLight, CCamera* aCamera, std::vector<CModelInstance*>& aModelList) {
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE bufferData;
	myFrameBufferData.myToCamera = aCamera->GetTransform();
	myFrameBufferData.myToProjection = aCamera->GetProjection();
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
	
	ID3D11ShaderResourceView* environmentLightShaderResource = anEnvironmentLight->GetCubeMap();	
	myContext->PSSetShaderResources(0, 1, &environmentLightShaderResource);

	for (CModelInstance* instance : aModelList) {
		CModel* model = instance->GetModel();
		CModel::SModelData modelData = model->GetModelData();

		myObjectBufferData.myToWorld = instance->GetTransform();
		//myObjectBufferData.myUVOffset = instance->GetUVOffset();

		ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
		result = myContext->Map(myObjectBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData);
		if (FAILED(result)) {
			return;
		}
		memcpy(bufferData.pData, &myObjectBufferData, sizeof(SObjectBufferData));
		myContext->Unmap(myObjectBuffer, 0);

		myContext->IASetPrimitiveTopology(modelData.myPrimitiveTopology);
		myContext->IASetInputLayout(modelData.myInputLayout);
		myContext->IASetVertexBuffers(0, 1, &modelData.myVertexBuffer, &modelData.myStride, &modelData.myOffset);
		myContext->IASetIndexBuffer(modelData.myIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		myContext->VSSetConstantBuffers(1, 1, &myObjectBuffer);
		myContext->VSSetShader(modelData.myVertexShader, nullptr, 0);

		myContext->PSSetShaderResources(1, 2, &modelData.myTexture[0]);
		myContext->PSSetSamplers(0, 1, &modelData.mySamplerState);
		myContext->PSSetShader(modelData.myPixelShader, nullptr, 0);

		myContext->DrawIndexed(modelData.myNumberOfIndicies, 0, 0);
	}
}