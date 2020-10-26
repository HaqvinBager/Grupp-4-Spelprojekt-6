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

#include <ParticleFactory.h>
#include <Particle.h>
#include <ParticleInstance.h>

#include <SpriteInstance.h>
#include <SpriteFactory.h>

#include <IntersectionManager.h>
#include <iostream>
#include <AnimationComponent.h>
#include <Animation.h>

#include <NavmeshLoader.h>
#include <MouseTracker.h>

#include "LevelLoader.h"

using namespace CommonUtilities;

CShowCase::CShowCase() : myLevelLoader(new CLevelLoader()), myPlayer(nullptr), myEnemy(nullptr), myCamera(nullptr), myFreeCamera(nullptr), myWindowWidth(0), myWindowHeight(0) {}

CShowCase::~CShowCase() {}

void CShowCase::Init()
{
	CNavmeshLoader* nav = new CNavmeshLoader();
	myNavMesh = nav->LoadNavmesh("NavTest_ExportedNavMesh.obj");

	DirectX::SimpleMath::Ray* ray = new DirectX::SimpleMath::Ray({ -1.44f, 1.5f, 1.44f }, { 0, -1, 0 });

	std::cout << "NavMesh Size:" << myNavMesh->myTriangles.size() << std::endl;
	for (size_t i = 0; i < myNavMesh->myTriangles.size(); i++)
	{
		std::cout << i << " NavMesh position X:" << myNavMesh->myTriangles[i]->myCenterPosition.x << " Y: " << myNavMesh->myTriangles[i]->myCenterPosition.y << " Z: " << myNavMesh->myTriangles[i]->myCenterPosition.z << std::endl;
	}
	STriangle* tri = myNavMesh->myTriangles[0];

	float dist = 0;
	if (ray->Intersects({ tri->myVertexPositions[0] }, { tri->myVertexPositions[1] }, { tri->myVertexPositions[2] }, dist)) {
		std::cout << "IN" << std::endl;
	}
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

	//CSpriteFactory* spriteFactory = CSpriteFactory::GetInstance();
	//CSpriteInstance* spriteInstance = new CSpriteInstance();
	//spriteInstance->Init(spriteFactory->GetSprite("Texture.dds"));
	//CScene::GetInstance()->AddInstance(spriteInstance);
}

void CShowCase::Update()
{
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
		DirectX::SimpleMath::Vector4 origin = MouseTracker::ScreenPositionToWorldPosition(static_cast<float>(Input::GetInstance()->MouseX()), static_cast<float>(Input::GetInstance()->MouseY()), static_cast<float>(myWindowWidth), static_cast<float>(myWindowHeight));
		DirectX::SimpleMath::Vector3 cameraForward = CScene::GetInstance()->GetMainCamera()->GetTransform().Forward();
		DirectX::SimpleMath::Ray* ray = new DirectX::SimpleMath::Ray({ origin.x, origin.y, origin.z }, cameraForward);

		std::cout << "Origin \nX: " << origin.x << std::endl << "Y: " << origin.y << std::endl << "Z: " << origin.z << std::endl;
		std::cout << "\nDirection \nX: " << cameraForward.x << std::endl << "Y: " << cameraForward.y << std::endl << "Z: " << cameraForward.z << std::endl;
		
		float distToMesh = 0;
		for (auto& tri : myNavMesh->myTriangles)
		{
			if (ray->Intersects({ tri->myVertexPositions[0] }, { tri->myVertexPositions[1] }, { tri->myVertexPositions[2] }, distToMesh)) {
				std::cout << "Collided with NavMesh!" << std::endl;
			}
		}
	}
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
