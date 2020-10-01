#include "stdafx.h"
#include <array>
#include "Engine.h"
#include "WindowHandler.h"
#include "DirectXFramework.h"
#include "ForwardRenderer.h"
#include "Scene.h"
#include "Camera.h"
#include "Timer.h"
#include "ModelFactory.h"
#include "CameraFactory.h"
#include "LightFactory.h"
#include "EnvironmentLight.h"
#include "PointLight.h"

#include <string>

#pragma comment(lib, "d3d11.lib")

CEngine::CEngine()
{
	myScene = new CScene();	
	myWindowHandler = new CWindowHandler();
	myFramework = new CDirectXFramework();
	myForwardRenderer = new CForwardRenderer();
	myTimer = new CTimer();
	myModelFactory = new CModelFactory();
	myCameraFactory = new CCameraFactory();
	myLightFactory = new CLightFactory();
}

CEngine::~CEngine()
{
	delete myScene;
	myScene = nullptr;
	delete myWindowHandler;
	myWindowHandler = nullptr;
	delete myFramework;
	myFramework = nullptr;
	delete myTimer;
	myTimer = nullptr;

	delete myModelFactory;
	myModelFactory = nullptr;
	delete myCameraFactory;
	myCameraFactory = nullptr;
	delete myLightFactory;
	myLightFactory = nullptr;
}

bool CEngine::Init(CWindowHandler::SWindowData& someWindowData)
{
	ENGINE_ERROR_BOOL_MESSAGE(myWindowHandler->Init(someWindowData), "Window Handler could not be initialized.");

	ENGINE_ERROR_BOOL_MESSAGE(myFramework->Init(myWindowHandler), "Framework could not be initialized.");

	ENGINE_ERROR_BOOL_MESSAGE(myForwardRenderer->Init(*this), "Forward Renderer could not be initiliazed.");

	ENGINE_ERROR_BOOL_MESSAGE(myModelFactory->Init(*this), "Model Factory could not be initiliazed.");

	ENGINE_ERROR_BOOL_MESSAGE(myCameraFactory->Init(myWindowHandler), "Camera Factory could not be initialized.");

	ENGINE_ERROR_BOOL_MESSAGE(myScene->Init(), "Scene could not be initialized.");

	ENGINE_ERROR_BOOL_MESSAGE(myLightFactory->Init(*this), "Light Factory could not be initialized.");

	return true;
}

float CEngine::BeginFrame()
{
	std::array<float, 4> clearColor = { 0.5f, 0.5f, 0.5f, 1.0f };
	myFramework->BeginFrame(clearColor);
	return myTimer->Mark();
}

void CEngine::RenderFrame()
{
	CEnvironmentLight* environmentLight = myScene->GetEnvironmentLight();
	CCamera* mainCamera = myScene->GetMainCamera();
	
	std::vector<CModelInstance*> modelsToRender = myScene->CullModels(mainCamera);
	std::vector<std::pair<unsigned int, std::array<CPointLight*, 8>>> pointLights;
	for (CModelInstance* instance : modelsToRender)
	{
		pointLights.push_back(myScene->CullLights(instance));
	}

	myForwardRenderer->Render(environmentLight, pointLights, mainCamera, modelsToRender);
}

void CEngine::EndFrame()
{
	myFramework->EndFrame();
}

CWindowHandler* CEngine::GetWindowHandler()
{
	return myWindowHandler;
}
