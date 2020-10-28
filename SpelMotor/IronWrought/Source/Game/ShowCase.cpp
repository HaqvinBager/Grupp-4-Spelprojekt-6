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
#include <ModelFactory.h>
#include <ModelComponent.h>

#include <ParticleFactory.h>
#include <Particle.h>
#include <ParticleInstance.h>

#include <SpriteInstance.h>
#include <SpriteFactory.h>

#include <IntersectionManager.h>
#include <iostream>
#include <AnimationComponent.h>
#include <Animation.h>
#include <InputMapper.h>
#include "PlayerControllerComponent.h"

#include <NavmeshLoader.h>
#include <AStar.h>
#include <MouseTracker.h>

#include <LineFactory.h>
#include <LineInstance.h>

#include "LevelLoader.h"

#include <Debug.h>
#include <algorithm>

using namespace CommonUtilities;

CShowCase::CShowCase() : myLevelLoader(new CLevelLoader()), myPlayer(nullptr), myEnemy(nullptr), myCamera(nullptr), myFreeCamera(nullptr), myWindowWidth(0), myWindowHeight(0) {}

CShowCase::~CShowCase() {}

void CShowCase::Init()
{
	CNavmeshLoader* nav = new CNavmeshLoader();
	myNavMesh = nav->LoadNavmesh("NavTest_ExportedNavMesh.obj");

	myLevelLoader->Init();

	//myLevelLoader->LoadNewLevel("TODO");

	myPlayer = CreatePlayer({ 0.0f, 0.0f, 0.0f });
	myEnemy = CreateEnemy({ 0.5f, 0.0f, 0.0f });

	myCamera = CreateCamera(myPlayer);
	CParticle* particlePrefab = CParticleFactory::GetInstance()->LoadParticle("ParticleData_SmokeEmitter.json");
	CParticleInstance* particleEmitter = new CParticleInstance();
	particleEmitter->Init(particlePrefab);
	CScene::GetInstance()->AddInstance(particleEmitter);

	myFreeCamera = CreateCamera(nullptr);
	//TODO TEMPORARY REMOVE MOvE YES
	CInputMapper::GetInstance()->MapEvent(CInputObserver::EInputAction::MouseLeft, CInputObserver::EInputEvent::MoveClick);
	CInputMapper::GetInstance()->MapEvent(CInputObserver::EInputAction::MouseRight, CInputObserver::EInputEvent::AttackClick);
	//TODO TEMPORARY REMOVE MOVE YES
	//CSpriteFactory* spriteFactory = CSpriteFactory::GetInstance();
	//CSpriteInstance* spriteInstance = new CSpriteInstance();
	//spriteInstance->Init(spriteFactory->GetSprite("Texture.dds"));
	//CScene::GetInstance()->AddInstance(spriteInstance);
}

void CShowCase::Update()
{
	/*
	* Debug.DrawLine(positionA, positionB, color);
		ENGINE_DRAW_LINE(posA, posB, color);
	*/

	Vector3 playerPosition = myPlayer->GetComponent<CTransformComponent>()->Position();
	Vector3 enemyPosition = myEnemy->GetComponent<CTransformComponent>()->Position();

	CDebug::GetInstance()->DrawLine(playerPosition, enemyPosition);


	//CLineInstance* line = new CLineInstance();
	//line->Init(CLineFactory::GetInstance()->CreateLine(fromTest, toTest, { 0.1f, 255.0f, 0.1f, 1.0f }));
	//CScene::GetInstance()->AddInstance(line);


	myCamera->Update();
	if (Input::GetInstance()->IsKeyPressed('F')) {
		if (CScene::GetInstance()->GetMainCamera() == myCamera) {
			CScene::GetInstance()->SetMainCamera(myFreeCamera);
		}
		else {
			CScene::GetInstance()->SetMainCamera(myCamera);
		}
	}
	UpdatePlayerController();

	if (Input::GetInstance()->IsMousePressed(Input::MouseButton::Left)) {
		DirectX::SimpleMath::Ray ray = MouseTracker::WorldSpacePick(myWindowWidth, myWindowHeight);

		CLineInstance* lineInstance = new CLineInstance();
		DirectX::SimpleMath::Vector3 to = ray.position + ray.direction * 100.0f;
		lineInstance->Init(CLineFactory::GetInstance()->CreateLine({ ray.position.x, ray.position.y, ray.position.z }, to, {255.f, 0.f ,0.f, 1.f}));
		CScene::GetInstance()->AddInstance(lineInstance);

		float distToMesh = 0;
		for (auto& tri : myNavMesh->myTriangles)
		{
			if (ray.Intersects({ tri->myVertexPositions[0] }, { tri->myVertexPositions[1] }, { tri->myVertexPositions[2] }, distToMesh)) {
				DirectX::SimpleMath::Vector3 pickedPosition = ray.position + ray.direction * distToMesh;

				STriangle* playerPos = myNavMesh->GetTriangleAtPoint(myPlayer->GetComponent<CTransformComponent>()->Position());
				STriangle* pickedTriangle = myNavMesh->GetTriangleAtPoint(pickedPosition);
				myPlayer->GetComponent<CTransformComponent>()->ClearPath();
				std::vector<DirectX::SimpleMath::Vector3> aPath;
				
				if (playerPos != pickedTriangle) {
					aPath = CAStar::AStar(myNavMesh, playerPos, tri);
				}
				
				myPlayer->GetComponent<CTransformComponent>()->SetPath(aPath, pickedPosition);
			}
		}
	}

	myPlayer->GetComponent<CTransformComponent>()->MoveAlongPath();
}

CGameObject* CShowCase::CreatePlayer(Vector3 aPosition)
{
	CGameObject* player = new CGameObject();
	CTransformComponent* transform = player->AddComponent<CTransformComponent>(*player, aPosition);
	transform->Scale(1.0f);
	transform->Position(aPosition);

	transform->Rotation({ 0.0f, 180.0f, 0.0f });
	player->AddComponent<CCapsuleColliderComponent>(*player, 0.35f, 2.0f);
	player->AddComponent<CModelComponent>(*player, "Assets/3D/Character/Npc1/CH_NPC_woman_01_19G4_1_19.fbx");
	player->AddComponent<CPlayerControllerComponent>(*player);
	//model->SetMyModel(CModelFactory::GetInstance()->GetModelPBR());
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
	auto modelComponent = enemy->AddComponent<CModelComponent>(*enemy, "ani/CH_NPC_Undead_17G3_SK.fbx");

	CAnimationComponent* animation = enemy->AddComponent<CAnimationComponent>(*enemy);
	std::vector<std::string> somePathsToAnimations;
	somePathsToAnimations.push_back("ani/CH_NPC_Undead@Walk_01_17G3_AN.fbx");
	somePathsToAnimations.push_back("ani/CH_NPC_Undead@Idle_01_17G3_AN.fbx");

	const std::string rigModel = "Ani/CH_NPC_Undead_17G3_SK.fbx";
	animation->GetMyAnimation()->Init(rigModel.c_str(), somePathsToAnimations);
	modelComponent->GetMyModel()->AddAnimation(animation->GetMyAnimation());
	animation->SetBlend(0, 1, 1.0f);

	CScene::GetInstance()->AddInstance(enemy);
	return enemy;
}

CCamera* CShowCase::CreateCamera(CGameObject* aCameraTarget)
{
	CCamera* camera = CCameraFactory::GetInstance()->CreateCamera(45.0f);
	if (aCameraTarget) {
		camera->SetTarget(aCameraTarget->GetComponent<CTransformComponent>(), { -0.05f, 10.5f, -7.0f }, { 50.0f, 0.0f, 0.0f });
		camera->SetPosition(aCameraTarget->GetComponent<CTransformComponent>()->Position());
	}
	else {
		camera->SetPosition({0.0f, 0.0f, 0.0f});
	}
	//camera->Move({ 1.5f, 0.0f, -2.0f });
	CScene::GetInstance()->AddInstance(camera);
	if (aCameraTarget) {
		CScene::GetInstance()->SetMainCamera(camera);
	}
	return camera;
}

void CShowCase::UpdatePlayerController()
{
	if (CScene::GetInstance()->GetMainCamera() == myCamera) {
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
	}

	if (CScene::GetInstance()->GetMainCamera() == myFreeCamera) {
		const float dt = CTimer::Dt();
		float cameraMoveSpeed = 20.0f;
		float verticalMoveSpeedModifier = 0.5f;
		Vector3 camera_movement_input(0, 0, 0);
		camera_movement_input.z = Input::GetInstance()->IsKeyDown('W') ? cameraMoveSpeed : camera_movement_input.z;
		camera_movement_input.z = Input::GetInstance()->IsKeyDown('S') ? -cameraMoveSpeed : camera_movement_input.z;
		camera_movement_input.x = Input::GetInstance()->IsKeyDown('D') ? cameraMoveSpeed : camera_movement_input.x;
		camera_movement_input.x = Input::GetInstance()->IsKeyDown('A') ? -cameraMoveSpeed : camera_movement_input.x;
		camera_movement_input.y = Input::GetInstance()->IsKeyDown(VK_SPACE) ? cameraMoveSpeed * verticalMoveSpeedModifier : camera_movement_input.y;
		camera_movement_input.y = Input::GetInstance()->IsKeyDown(VK_SHIFT) ? -cameraMoveSpeed * verticalMoveSpeedModifier : camera_movement_input.y;

		float rotationSpeed = 2.5f;
		float camera_rotation_input = 0;
		camera_rotation_input = Input::GetInstance()->IsKeyDown('Q') ? -rotationSpeed : camera_rotation_input;
		camera_rotation_input = Input::GetInstance()->IsKeyDown('E') ? rotationSpeed : camera_rotation_input;

		CCamera* camera = CScene::GetInstance()->GetMainCamera();
		camera->Move(camera_movement_input * dt);
		camera->Rotate({ 0, camera_rotation_input * dt, 0 });
	}



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
