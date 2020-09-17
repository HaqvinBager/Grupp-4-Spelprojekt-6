#include "stdafx.h"
#include "EnvironmentLight.h"
#include "DirectXFramework.h"
#include <SimpleMath.h>
#include <DDSTextureLoader.h>

CEnvironmentLight::CEnvironmentLight()
{
}

CEnvironmentLight::~CEnvironmentLight()
{

}

bool CEnvironmentLight::Init(CDirectXFramework* aFramework, std::string aFilePath)
{

	//D3D11_SHADER_RESOURCE_VIEW_DESC SMViewDesc;
	//SMViewDesc.Format = D3D11_TEXCUBE_SRV;// SMTextureDesc.Format;
	//SMViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	//SMViewDesc.TextureCube.MipLevels = SMTextureDesc.MipLevels;
	//SMViewDesc.TextureCube.MostDetailedMip = 0;

	//hr = d3d11Device->CreateShaderResourceView(SMTexture, &SMViewDesc, &smrv);*/


	HRESULT result;
	wchar_t* cubeMapPathWide = new wchar_t[aFilePath.length() + 1];
	std::copy(aFilePath.begin(), aFilePath.end(), cubeMapPathWide);
	cubeMapPathWide[aFilePath.length()] = 0;

	result = DirectX::CreateDDSTextureFromFile(aFramework->GetDevice(), cubeMapPathWide, nullptr, &myCubeMap);
	if (FAILED(result))
	{
		return false;
	}

	delete[] cubeMapPathWide;
	return true;
}

void CEnvironmentLight::SetDirection(DirectX::SimpleMath::Vector3 aDirection)
{
	myDirection = DirectX::SimpleMath::Vector4(aDirection.x, aDirection.y, aDirection.z, 0.0f);
}

void CEnvironmentLight::SetColor(DirectX::SimpleMath::Vector3 aColor)
{
	myColor = DirectX::SimpleMath::Vector4(aColor.x, aColor.y, aColor.z, 1.0f);
}

ID3D11ShaderResourceView* CEnvironmentLight::GetCubeMap()
{
	return myCubeMap;
}
