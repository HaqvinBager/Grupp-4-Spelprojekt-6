#include "stdafx.h"
#include "Game.h"
#include <SimpleMath.h>
#include <Scene.h>
#include <CameraFactory.h>
#include <Camera.h>
#include <Model.h>
#include <ModelFactory.h>
#include <ModelInstance.h>
#include <Input.h>
#include <LightFactory.h>
#include <EnvironmentLight.h>
#include <Timer.h>
#include "LevelLoader.h"

#include <GameObject.h>
#include <Component.h>
#include <TransformComponent.h>
#include <ModelComponent.h>
#include <AnimationComponent.h>
//#include <MouseTracker.h>
#include <AnimationController.h>

#include <Animation.h>
#include <iostream>
#include <CapsuleColliderComponent.h>
#include <IntersectionManager.h>


#ifdef _DEBUG
#pragma comment(lib, "Engine_Debug.lib")
#pragma comment(lib, "UnityImporterApp_Debug.lib")
#endif // _DEBUG
#ifdef NDEBUG
#pragma comment(lib, "Engine_Release.lib")
#endif // NDEBUG

namespace SM = DirectX::SimpleMath;
using namespace CommonUtilities;

CGame::CGame() 
	: myLevelLoader(new CLevelLoader())
{
}

CGame::~CGame()
{
}

void CGame::Init()
{
	myLevelLoader->Init();
	//myLevelLoader->LoadNewLevel("Levels/SampleScene_exportedLevelASCII.txt");
	CScene* scene = CScene::GetInstance();
	CLightFactory* lightFactory = CLightFactory::GetInstance();

	CCamera* camera = CCameraFactory::GetInstance()->CreateCamera(65.0f);

	/*CModelInstance* cube = new CModelInstance();
	CModel* cubeModel = CModelFactory::GetInstance()->GetCube();
	cube->Init(cubeModel);*/

	CEnvironmentLight* environmentLight = lightFactory->CreateEnvironmentLight("Yokohama2.dds");
	environmentLight->SetDirection(SM::Vector3(0, 1, -1));
	environmentLight->SetColor(SM::Vector3(0.8f, 0.8f, 0.8f));

	scene->AddInstance(camera);
	scene->AddInstance(environmentLight);

	//scene->AddInstance(cube);
	scene->SetMainCamera(camera);


	//GameObjects Boi
	CGameObject* player = new CGameObject();
	/*CTransformComponent* transfComp =*/
	player->AddComponent<CTransformComponent>(*player);
	CModelComponent* modelComponent = player->AddComponent<CModelComponent>(*player);
	CModel* playerModel = CModelFactory::GetInstance()->LoadModelPBR("Assets/3D/Character/Npc1/CH_NPC_woman_01_19G4_1_19.fbx");
	modelComponent->SetMyModel(playerModel);
	player->GetComponent<CTransformComponent>()->Scale(1.f);

	//CAnimationComponent* animComp = player->AddComponent<CAnimationComponent>(*player);
	//std::vector<std::string> somePathsToAnimations;
	//somePathsToAnimations.push_back("ani/CH_NPC_Undead@Walk_01_17G3_AN.fbx");
	//somePathsToAnimations.push_back("ani/CH_NPC_Undead@Idle_01_17G3_AN.fbx");
	//
	//const std::string rigModel = "Ani/CH_NPC_Undead_17G3_SK.fbx";
	//animComp->GetMyAnimation()->Init(rigModel.c_str(), somePathsToAnimations);
	//animModel->AddAnimation(animComp->GetMyAnimation());
	//animComp->SetBlend(0, 1, 1.0f);

	player->AddComponent<CCapsuleColliderComponent>(*player, 0.5f, 2.0f);

	CGameObject* otherBoi = new CGameObject();
	otherBoi->AddComponent<CTransformComponent>(*otherBoi);
	otherBoi->AddComponent<CCapsuleColliderComponent>(*otherBoi, 0.5f, 2.0f);

	scene->AddInstance(player);
	scene->AddInstance(otherBoi);
	//!GameObjects Boi

	std::vector<CGameObject*> gameObjects = CScene::GetInstance()->CullGameObjects(CScene::GetInstance()->GetMainCamera());
	for (auto& gameobject : gameObjects)
	{
		gameobject->Awake();
	}

	for (auto& gameobject : gameObjects)
	{
		gameobject->Start();
	}
}

void CGame::Update()
{
	//Axel, Alexander M och Gustav Did this. :)
	//Updaterar alla GameObjects så deras komponenter blir uppdaterade
	//Gjorde detta så CapsuleColliders Position etc uppdateras!
	std::vector<CGameObject*> gameObjects = CScene::GetInstance()->CullGameObjects(CScene::GetInstance()->GetMainCamera());
	for (auto& gameobject : gameObjects)
	{
		gameobject->Update();
	}

	auto gameobjects = CScene::GetInstance()->CullGameObjects(CScene::GetInstance()->GetMainCamera());
	if (Input::GetInstance()->IsKeyDown(VK_UP))
	{
		gameobjects[0]->GetComponent<CTransformComponent>()->Move({ 0.0f, 0.0f, 100.0f * CTimer::Dt() });
	}
	if (Input::GetInstance()->IsKeyDown(VK_DOWN))
	{
		gameobjects[0]->GetComponent<CTransformComponent>()->Move({ 0.0f, 0.0f, -100.0f * CTimer::Dt() });
	}
	if (Input::GetInstance()->IsKeyDown(VK_LEFT))
	{
		gameobjects[0]->GetComponent<CTransformComponent>()->Move({ -100.0f * CTimer::Dt(), 0.0f , 0.0f });
	}
	if (Input::GetInstance()->IsKeyDown(VK_RIGHT))
	{
		gameobjects[0]->GetComponent<CTransformComponent>()->Move({ 100.0f * CTimer::Dt(), 0.0f , 0.0f });
	}

	UpdateCamera();
	/*auto models = CScene::GetInstance()->CullGameObjects(CScene::GetInstance()->GetMainCamera());
	for (CGameObject* go : models)
	{
		if (go->GetComponent<CAnimationComponent>())
		{
			go->GetComponent<CAnimationComponent>()->SetBlend(0, 1, sinf(CTimer::Time()));
			go->GetComponent<CAnimationComponent>()->Update();
		}
	}



	if (Input::GetInstance()->IsKeyPressed('1')) {
		myLevelLoader->LoadNewLevel("Levels/Dungeon_exportedLevelASCII.txt");
	}
	if (Input::GetInstance()->IsKeyPressed('2')) {
		myLevelLoader->LoadNewLevel("Levels/Gardens_exportedLevelASCII.txt");
	}
	if (Input::GetInstance()->IsKeyPressed('3')) {
		myLevelLoader->LoadNewLevel("Levels/Castle_exportedLevelASCII.txt");
	}
	if (Input::GetInstance()->IsKeyPressed('4')) {
		myLevelLoader->LoadNewLevel("Levels/SampleScene_exportedLevelASCII.txt");
	}
	if (Input::GetInstance()->IsKeyPressed('5')) {
		myLevelLoader->LoadNewLevel("Levels/SampleScene_exportedLevelASCII_Smily.txt");
	}*/
}

void CGame::CapsuleColliderTest()
{
	std::vector<CGameObject*> gameObjects = CScene::GetInstance()->CullGameObjects(CScene::GetInstance()->GetMainCamera());
	float moveSpeed = 0.1f;
	CTransformComponent* boi = gameObjects[0]->GetComponent<CTransformComponent>();
	CTransformComponent* otherBoi = gameObjects[1]->GetComponent<CTransformComponent>();

	if (Input::GetInstance()->IsKeyDown('W'))
	{
		otherBoi->Move({ 0.0f, moveSpeed * CTimer::Dt(), 0.0f });
	}
	if (Input::GetInstance()->IsKeyDown('S'))
	{
		otherBoi->Move({ 0.0f, -moveSpeed * CTimer::Dt(), 0.0f });
	}
	if (Input::GetInstance()->IsKeyDown('A'))
	{
		otherBoi->Move({ -moveSpeed * CTimer::Dt(), 0.0f , 0.0f });
	}
	if (Input::GetInstance()->IsKeyDown('D'))
	{
		otherBoi->Move({ moveSpeed * CTimer::Dt(), 0.0f , 0.0f });
	}

	if (Input::GetInstance()->IsKeyDown(VK_UP))
	{
		boi->Move({ 0.0f, moveSpeed * CTimer::Dt(), 0.0f });
	}
	if (Input::GetInstance()->IsKeyDown(VK_DOWN))
	{
		boi->Move({ 0.0f, -moveSpeed * CTimer::Dt(), 0.0f });
	}
	if (Input::GetInstance()->IsKeyDown(VK_LEFT))
	{
		boi->Move({ -moveSpeed * CTimer::Dt(), 0.0f , 0.0f });
	}
	if (Input::GetInstance()->IsKeyDown(VK_RIGHT))
	{
		boi->Move({ moveSpeed * CTimer::Dt(), 0.0f , 0.0f });
	}


	if (CIntersectionManager::CapsuleIntersection(*boi->GetComponent<CCapsuleColliderComponent>(), *otherBoi->GetComponent<CCapsuleColliderComponent>()))
	{
		std::cout << "Boi		[X: " << boi->Position().x << ", Y: " << boi->Position().y << ", Z: " << boi->Position().z << "]" << " - ";
		std::cout << "\nOtherBoi	[X: " << otherBoi->Position().x << ", Y: " << otherBoi->Position().y << ", Z: " << otherBoi->Position().z << "]" << std::endl;
	}
}

void CGame::UpdateCamera()
{
	const float dt = CTimer::Dt();
	float cameraMoveSpeed = 20.0f;
	float verticalMoveSpeedModifier = 0.5f;
	SM::Vector3 camera_movement_input(0, 0, 0);
	camera_movement_input.z = Input::GetInstance()->IsKeyDown('W') ? cameraMoveSpeed : camera_movement_input.z;
	camera_movement_input.z = Input::GetInstance()->IsKeyDown('S') ? -cameraMoveSpeed : camera_movement_input.z;
	camera_movement_input.x = Input::GetInstance()->IsKeyDown('D') ? cameraMoveSpeed : camera_movement_input.x;
	camera_movement_input.x = Input::GetInstance()->IsKeyDown('A') ? -cameraMoveSpeed : camera_movement_input.x;
	camera_movement_input.y = Input::GetInstance()->IsKeyDown('R') ? cameraMoveSpeed * verticalMoveSpeedModifier : camera_movement_input.y;
	camera_movement_input.y = Input::GetInstance()->IsKeyDown('F') ? -cameraMoveSpeed * verticalMoveSpeedModifier : camera_movement_input.y;

	float rotationSpeed = 5.0f;
	float camera_rotation_input = 0;
	camera_rotation_input = Input::GetInstance()->IsKeyDown('Q') ? -rotationSpeed : camera_rotation_input;
	camera_rotation_input = Input::GetInstance()->IsKeyDown('E') ? rotationSpeed : camera_rotation_input;

	CCamera* camera = CScene::GetInstance()->GetMainCamera();
	camera->Move(camera_movement_input * dt);
	camera->Rotate({ 0, camera_rotation_input * dt, 0 });

	//CModelInstance* model = CScene::GetInstance()->CullModels(CScene::GetInstance()->GetMainCamera())[0];
	//model->Rotate({ 0.0f, sinf(CTimer::Time()) * dt, 0.0f });
	//model = nullptr;
}