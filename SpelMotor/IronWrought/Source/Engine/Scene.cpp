#include "stdafx.h"
#include "Scene.h"
#include "ModelInstance.h"
#include "EnvironmentLight.h"
#include "PointLight.h"
#include "ModelInstance.h"

CScene* CScene::ourInstance = nullptr;

CScene* CScene::GetInstance()
{
	return ourInstance;
}

CScene::CScene()
{
	ourInstance = this;
	myMainCamera = nullptr;
}

CScene::~CScene()
{
	ourInstance = nullptr;
	myMainCamera = nullptr;
}


bool CScene::Init()
{
	return true;
}


void CScene::SetMainCamera(CCamera* aCamera)
{
	myMainCamera = aCamera;
}

CCamera* CScene::GetMainCamera()
{
	return myMainCamera;
}

CEnvironmentLight* CScene::GetEnvironmentLight()
{
	return myEnvironmentLights[0];
}


std::vector<CModelInstance*> CScene::CullModels(CCamera* /*aMainCamera*/)
{

	return myModelInstances;
}

const std::vector<CPointLight*>& CScene::GetPointLights()
{
	return myPointLights;
}

std::pair<unsigned int, std::array<CPointLight*, 8>> CScene::CullLights(CModelInstance* aModel)
{
	std::pair<unsigned int, std::array<CPointLight*, 8>> pointLightPair;
	UINT counter = 0;
	for (unsigned int i = 0; i < myPointLights.size() && counter < 8; ++i)
	{
		DirectX::SimpleMath::Vector3 pointLightPosition;
		pointLightPosition.x = myPointLights[i]->GetPosition().x;
		pointLightPosition.y = myPointLights[i]->GetPosition().y;
		pointLightPosition.z = myPointLights[i]->GetPosition().z;

		float distSqr = DirectX::SimpleMath::Vector3::DistanceSquared(pointLightPosition, aModel->GetTransform().Translation());
		float pointLightRange = myPointLights[i]->GetRange();
		if (distSqr < pointLightRange * pointLightRange)
		{
			pointLightPair.second[counter] = myPointLights[i];
			counter++;
		}
	}
	pointLightPair.first = counter;
	return pointLightPair;
}

bool CScene::AddInstance(CModelInstance* aModel)
{
	myModelInstances.emplace_back(aModel);
	return true;
}

bool CScene::AddInstance(CCamera* aCamera)
{
	myCameras.emplace_back(aCamera);
	return true;
}

bool CScene::AddInstance(CEnvironmentLight* anEnvironmentLight)
{
	myEnvironmentLights.emplace_back(anEnvironmentLight);
	return true;
}

bool CScene::AddInstance(CPointLight* aPointLight)
{
	myPointLights.emplace_back(aPointLight);
	return true;
}
