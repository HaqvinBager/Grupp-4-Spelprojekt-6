#include "stdafx.h"
#include "Scene.h"
#include "ModelInstance.h"
#include "EnvironmentLight.h"

CScene* CScene::ourInstance = nullptr;

CScene* CScene::GetInstance() {
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


bool CScene::Init() {
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


std::vector<CModelInstance*> CScene::CullModels(CCamera* /*aMainCamera*/) {
	
	return myModelInstances;
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
