#include "stdafx.h"
#include "PauseState.h"
#include "Canvas.h"
#include "MainSingleton.h"
#include "Scene.h"
#include "Engine.h"
#include "TransformComponent.h"
#include "EnviromentLightComponent.h"
#include "CameraControllerComponent.h"
#include "CameraComponent.h"

CPauseState::CPauseState(CStateStack& aStateStack): CState(aStateStack), myCanvas(nullptr) {
	myScene = new CScene();
	CEngine::GetInstance()->AddScene(myScene);
	unsigned int index = static_cast<unsigned int>(CEngine::GetInstance()->myScenes.size() - 1);
	CEngine::GetInstance()->SetActiveScene(index);

	CGameObject* camera = new CGameObject();
	camera->AddComponent<CCameraComponent>(*camera, 70.0f);
	camera->AddComponent<CCameraControllerComponent>(*camera, 25.0f);
	camera->myTransform->Position({ 0.0f, 0.0f, 0.0f });
	camera->myTransform->Rotation({ 0.0f, 0.0f, 0.0f });
	myScene->AddInstance(camera);
	myScene->SetMainCamera(camera->GetComponent<CCameraComponent>());

	CGameObject* envLight = new CGameObject();
	envLight->AddComponent<CEnviromentLightComponent>(*envLight);
	myScene->AddInstance(envLight);
	myScene->SetEnvironmentLight(envLight->GetComponent<CEnviromentLightComponent>()->GetEnviromentLight());

	myCanvas = new CCanvas();
	myCanvas->Init("Json/UI_PauseMenu_Description.json");

	myState = CStateStack::EStates::PauseMenu;
}

CPauseState::~CPauseState() {
	CMainSingleton::PostMaster().Unsubscribe(EMessageType::MainMenu,this);
	CMainSingleton::PostMaster().Unsubscribe(EMessageType::Resume,this);
	CEngine::GetInstance()->PopBackScene();
	//CEngine::GetInstance()->GetActiveScene().ClearScene();
	//CEngine::GetInstance()->GetActiveScene().ClearSprites();
}

void CPauseState::Awake() {
	CMainSingleton::PostMaster().Subscribe(EMessageType::MainMenu, this);
	CMainSingleton::PostMaster().Subscribe(EMessageType::Resume, this);
}

void CPauseState::Start()  {
}

void CPauseState::Update() {
	myCanvas->Update();
}

#include <iostream>
void CPauseState::Receive(const SMessage& aMessage) {
	if (this == myStateStack.GetTop()) {
		switch (aMessage.myMessageType) {
		case EMessageType::MainMenu:
			myStateStack.PopUntil(CStateStack::EStates::MainMenu);
			break;
		case EMessageType::Resume:
			std::cout << "Should not pop yet" << std::endl;
			myStateStack.PopState();
			break;
		default:
			break;
		}
	}
}

void CPauseState::MakeSceneActive() {
	CEngine::GetInstance()->SetActiveScene(myActiveScene);

}