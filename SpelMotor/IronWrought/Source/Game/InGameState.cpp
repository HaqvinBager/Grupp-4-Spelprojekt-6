#include "stdafx.h"
#include "InGameState.h"
#include "StateStack.h"
#include "Scene.h"
#include "GameObject.h"
#include "SpriteInstance.h"
#include "SpriteFactory.h"
#include "AnimationComponent.h"
#include "ModelComponent.h"
#include "Model.h"
#include "Animation.h"
#include "TransformComponent.h"
#include "DialogueSystem.h"
#include "Timer.h"
#include "CameraComponent.h"
#include "Engine.h"
#include "WindowHandler.h"
#include "AnimatedUIElement.h"
#include "InputMapper.h"
#include "PostMaster.h"
#include "Canvas.h"
#include "AbilityComponent.h"
//collider test
#include "CircleColliderComponent.h"
#include "RectangleColliderComponent.h"
#include "TriangleColliderComponent.h"
//collider test

#include "TokenPool.h"
#include "StatsComponent.h"
#include "EnemyBehavior.h"
#include "PlayerControllerComponent.h"
#include "AIBehaviorComponent.h"
#include "TransformComponent.h"
#include "PauseState.h"

CInGameState::CInGameState(CStateStack& aStateStack) : CState(aStateStack) {
	myCanvas = new CCanvas();
	myCanvas->Init("Json/UI_InGame_Description.json");
	myActiveScene = CEngine::GetInstance()->ScenesSize();

	CInputMapper::GetInstance()->AddObserver(IInputObserver::EInputEvent::PauseGame, this);
	myTokenPool = new CTokenPool(4, 4.0f);
}

CInGameState::~CInGameState()
{
	CEngine::GetInstance()->PopBackScene();

	CInputMapper::GetInstance()->AddObserver(IInputObserver::EInputEvent::PauseGame, this);
}
#include "AIBehaviorComponent.h"
void CInGameState::Awake()
{
	for (auto& gameObject : CEngine::GetInstance()->GetActiveScene().myGameObjects)
	{
		if (gameObject)
		{
			if (gameObject->GetComponent<CAIBehaviorComponent>())
			{
				int i = 0;
				i;
			}

			gameObject->Awake();
		}

	}

	//myEnemy = new CGameObject();
	//myEnemy->AddComponent<CModelComponent>(*myEnemy, "Assets/3D/Character/CH_NPC_enemy_01_19G4_1_19/CH_NPC_enemy_01_19G4_1_19.fbx");
	//myEnemy->AddComponent<CStatsComponent>(*myEnemy, 10.0f, 10.0f, 3.0f);
	//CEnemyBehavior* enemyBehavior = new CEnemyBehavior(&CEngine::GetInstance()->GetActiveScene().FindObjectOfType<CPlayerControllerComponent>()->GameObject());
	//myEnemy->AddComponent<CAIBehaviorComponent>(*myEnemy, enemyBehavior);
	//myEnemy->myTransform->Position({ 0.0f, 0.0f, 5.0f });
	//CEngine::GetInstance()->GetActiveScene().AddInstance(myEnemy);
}

void CInGameState::Start()
{
	for (auto& gameObject : CEngine::GetInstance()->GetActiveScene().myGameObjects)
	{
		gameObject->Start();
	}
}

void CInGameState::Update()
{
	CCollisionManager::GetInstance()->Update();

	myCanvas->Update();
	myTokenPool->GetInstance()->Update();

	for (auto& gameObject : CEngine::GetInstance()->GetActiveScene().myGameObjects)
	{
		gameObject->Update();

	}


}

void CInGameState::ReceiveEvent(const EInputEvent aEvent)
{
	if (this == myStateStack.GetTop()) {
		switch (aEvent) {
		case IInputObserver::EInputEvent::PauseGame:
			myStateStack.PushState(new CPauseState(myStateStack));
			myStateStack.Awake();
			myStateStack.Start();
			break;
		default:
			break;
		}
	}
}

void CInGameState::MakeSceneActive()
{
	CEngine::GetInstance()->SetActiveScene(myActiveScene);
}
