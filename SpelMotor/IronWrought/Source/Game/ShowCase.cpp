#include "stdafx.h"
#include "ShowCase.h"
#include "GameObject.h"
#include "Scene.h"
#include "Camera.h"
#include "Input.h"
#include "Timer.h"

#include <CameraFactory.h>
#include <CapsuleColliderComponent.h>
#include <Model.h>
#include <ModelInstance.h>
#include <ModelFactory.h>
#include <ModelComponent.h>

#include <IntersectionManager.h>
#include <iostream>
#include <AnimationComponent.h>
#include <Animation.h>

#include "LevelLoader.h"

using namespace CommonUtilities;

CShowCase::CShowCase() : myLevelLoader(new CLevelLoader()), myPlayer(nullptr), myEnemy(nullptr), myCamera(nullptr) {}

CShowCase::~CShowCase() {}

void CShowCase::Init()
{
	myLevelLoader->Init();
	//myLevelLoader->LoadNewLevel("TODO");

	myPlayer = CreatePlayer({ 0.0f, 0.0f, 0.0f });
	myEnemy = CreateEnemy({ 0.5f, 0.0f, 0.0f });
	myCamera = CreateCamera(myPlayer);
}

void CShowCase::Update()
{
	myCamera->Update();
	UpdatePlayerController();
}

CGameObject* CShowCase::CreatePlayer(Vector3 aPosition)
{
	CGameObject* player = new CGameObject();
	CTransformComponent* transform = player->AddComponent<CTransformComponent>(*player, aPosition);
	transform->Scale(1.0f);
	transform->Position(aPosition);

	transform->Rotation({ 0.0f, 180.0f, 0.0f });
	player->AddComponent<CCapsuleColliderComponent>(*player, 0.35f, 2.0f);
	CModelComponent* model = player->AddComponent<CModelComponent>(*player);
	model->SetMyModel(CModelFactory::GetInstance()->GetModelPBR("Assets/3D/Character/Npc1/CH_NPC_woman_01_19G4_1_19.fbx"));
	CScene::GetInstance()->AddInstance(player);
	return player;
}

CGameObject* CShowCase::CreateEnemy(Vector3 aPosition)
{
	CGameObject* enemy = new CGameObject();
	CTransformComponent* transform = enemy->AddComponent<CTransformComponent>(*enemy, aPosition);
	transform->Scale(1.0f);
	transform->Position(aPosition);

	transform->Rotation({ 0.0f, 0.0f, 0.0f });
	enemy->AddComponent<CCapsuleColliderComponent>(*enemy, 0.5f, 2.0f);
	CModelComponent* model = enemy->AddComponent<CModelComponent>(*enemy);
	model->SetMyModel(CModelFactory::GetInstance()->GetModelPBR("ani/CH_NPC_Undead_17G3_SK.fbx"));
	CAnimationComponent* animation = enemy->AddComponent<CAnimationComponent>(*enemy);	

	std::vector<std::string> somePathsToAnimations;
	somePathsToAnimations.push_back("ani/CH_NPC_Undead@Walk_01_17G3_AN.fbx");
	somePathsToAnimations.push_back("ani/CH_NPC_Undead@Idle_01_17G3_AN.fbx");
	
	const std::string rigModel = "Ani/CH_NPC_Undead_17G3_SK.fbx";
	animation->GetMyAnimation()->Init(rigModel.c_str(), somePathsToAnimations);
	model->GetMyModel()->GetModel()->AddAnimation(animation->GetMyAnimation());
	animation->SetBlend(0, 1, 1.0f);

	CScene::GetInstance()->AddInstance(enemy);
	return enemy;
}

CCamera* CShowCase::CreateCamera(CGameObject* aCameraTarget)
{
	CCamera* camera = CCameraFactory::GetInstance()->CreateCamera(45.0f);
	camera->SetTarget(aCameraTarget->GetComponent<CTransformComponent>(), { -0.05f, 10.5f, -7.0f }, { 50.0f, 0.0f, 0.0f });
	camera->SetPosition(aCameraTarget->GetComponent<CTransformComponent>()->Position());
	//camera->Move({ 1.5f, 0.0f, -2.0f });
	CScene::GetInstance()->AddInstance(camera);
	CScene::GetInstance()->SetMainCamera(camera);
	return camera;
}

void CShowCase::UpdatePlayerController()
{
	float playerMoveSpeed = 20.0f;

	playerMoveSpeed = Input::GetInstance()->IsKeyDown(VK_SHIFT) ? playerMoveSpeed * 1.5f : playerMoveSpeed;

	Vector3 input(0, 0, 0);
	input.z = Input::GetInstance()->IsKeyDown('W') ? -playerMoveSpeed : input.z;
	input.z = Input::GetInstance()->IsKeyDown('S') ? playerMoveSpeed : input.z;
	input.x = Input::GetInstance()->IsKeyDown('D') ? -playerMoveSpeed : input.x;
	input.x = Input::GetInstance()->IsKeyDown('A') ? playerMoveSpeed : input.x;
	
	CTransformComponent* playerTransform = myPlayer->GetComponent<CTransformComponent>();
	Vector3 lastPosition = playerTransform->Position();

	playerTransform->Move(input);

	//CCapsuleColliderComponent* playerCollider = myPlayer->GetComponent<CCapsuleColliderComponent>();
	//CCapsuleColliderComponent* enemyCollider = myEnemy->GetComponent<CCapsuleColliderComponent>();
	//if (CIntersectionManager::CapsuleIntersection(*playerCollider, *enemyCollider))
	//{
	//	//Direction from Enemy to Player
	//	CTransformComponent* enemyTransform = myEnemy->GetComponent<CTransformComponent>();
	//	Vector3 direction = (enemyTransform->Position() - playerTransform->Position());
	//	direction.Normalize();

	//	playerTransform->Position(lastPosition);
	//	playerTransform->Move(direction * playerMoveSpeed);

	//	//playerTransform->Move();
	//}
}


/*  const float dt = CTimer::Dt();
	float cameraMoveSpeed = 2.0f;
	float verticalMoveSpeedModifier = 0.5f;
	Vector3 camera_movement_input(0, 0, 0);
	camera_movement_input.z = Input::GetInstance()->IsKeyDown('W') ? cameraMoveSpeed : camera_movement_input.z;
	camera_movement_input.z = Input::GetInstance()->IsKeyDown('S') ? -cameraMoveSpeed : camera_movement_input.z;
	camera_movement_input.x = Input::GetInstance()->IsKeyDown('D') ? cameraMoveSpeed : camera_movement_input.x;
	camera_movement_input.x = Input::GetInstance()->IsKeyDown('A') ? -cameraMoveSpeed : camera_movement_input.x;
	camera_movement_input.y = Input::GetInstance()->IsKeyDown('R') ? cameraMoveSpeed * verticalMoveSpeedModifier : camera_movement_input.y;
	camera_movement_input.y = Input::GetInstance()->IsKeyDown('F') ? -cameraMoveSpeed * verticalMoveSpeedModifier : camera_movement_input.y;

	float rotationSpeed = 1.0f;
	float camera_rotation_input = 0;
	camera_rotation_input = Input::GetInstance()->IsKeyDown('Q') ? -rotationSpeed : camera_rotation_input;
	camera_rotation_input = Input::GetInstance()->IsKeyDown('E') ? rotationSpeed : camera_rotation_input;

	CCamera* camera = CScene::GetInstance()->GetMainCamera();
	camera->Move(camera_movement_input * dt);
	camera->Rotate({ 0, camera_rotation_input * dt, 0 });*/