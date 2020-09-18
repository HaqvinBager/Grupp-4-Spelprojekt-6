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

//void CEnvironmentLight::SetDirection(DirectX::SimpleMath::Vector3 aDirection)
//{
//	myDirection = { myDirection.x = aDirection.x, myDirection.y = aDirection.y, myDirection.z = aDirection.z };
//}
//
//void CEnvironmentLight::SetColor(DirectX::SimpleMath::Vector3 aColor)
//{
//	myColor = { aColor.x, aColor.y, aColor.z };
//}

ID3D11ShaderResourceView* CEnvironmentLight::GetCubeMap()
{
	return myCubeMap;
}
