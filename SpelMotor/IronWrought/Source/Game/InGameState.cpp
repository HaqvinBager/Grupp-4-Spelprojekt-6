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


CInGameState::CInGameState(CStateStack& aStateStack) : CState(aStateStack) {
	myCanvas = new CCanvas();
	myCanvas->Init("Json/UI_InGame_Description.json");

	myPlayer = new CGameObject();
	std::vector<std::pair<EAbilityType, unsigned int>> abilities;
	abilities.emplace_back(std::pair<EAbilityType, unsigned int>(EAbilityType::WHIRLWIND, 5));
	abilities.emplace_back(std::pair<EAbilityType, unsigned int>(EAbilityType::BOX, 7));
	abilities.emplace_back(std::pair<EAbilityType, unsigned int>(EAbilityType::TRIANGLE, 1));
	myPlayer->AddComponent<CAbilityComponent>(*myPlayer, abilities);
}

CInGameState::~CInGameState()
{

}

void CInGameState::Awake()
{
	for (auto& gameObject : CEngine::GetInstance()->GetActiveScene().myGameObjects)
	{
		gameObject->Awake();
	}
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
	myPlayer->Update();
	for (auto& gameObject : CEngine::GetInstance()->GetActiveScene().myGameObjects)
	{
		gameObject->Update();

	}


}