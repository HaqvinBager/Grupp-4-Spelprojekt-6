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
	if (!myWindowHandler->Init(someWindowData))
	{
		return false;
	}

	if (!myFramework->Init(myWindowHandler))
	{
		return false;
	}

	if (!myForwardRenderer->Init(*this))
	{
		return false;
	}

	if (!myModelFactory->Init(*this))
	{
		return false;
	}

	if (!myCameraFactory->Init(myWindowHandler))
	{
		return false;
	}

	if (!myScene->Init())
	{
		return false;
	}

	if (!myLightFactory->Init(*this))
	{
		return false;
	}

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
	if (mainCamera)
	{
		std::vector<CModelInstance*> modelsToRender = myScene->CullModels(mainCamera);
		myForwardRenderer->Render(environmentLight, mainCamera, modelsToRender);
	}
}

void CEngine::EndFrame()
{
	myFramework->EndFrame();
}

CWindowHandler* CEngine::GetWindowHandler()
{
	return myWindowHandler;
}
