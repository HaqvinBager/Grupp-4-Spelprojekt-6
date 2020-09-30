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
#include <PointLight.h>
#include "LevelLoader.h"

#include <Animation.h>


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
	myLevelLoader->LoadNewLevel("Levels/SampleScene_exportedLevelASCII.txt");
	CScene* scene = CScene::GetInstance();
	CLightFactory* lightFactory = CLightFactory::GetInstance();
	CEnvironmentLight* environmentLight = lightFactory->CreateEnvironmentLight("Yokohama2.dds");
	CPointLight* light = lightFactory->CreatePointLight();
	light->Init();

	//CModelInstance* model = CModelFactory::GetInstance()->CreateModel("Model/Chest/Particle_Chest.fbx", 0.025f);
	//model->SetPosition({ 12.5f, 0.0f, 15.0f });
	//camera->SetPosition({ 10.0f, 2.5f, 5.0f });
	light->SetPosition({ 10.0f, 1.0f, 7.0f });
	//
	environmentLight->SetDirection(SM::Vector3(0, 1, -1));
	environmentLight->SetColor(SM::Vector3(0.8f, 0.8f, 0.8f));
	//
	light->SetColor({ 1.0f, 0.2f, 0.1f });
	light->SetRange(10.0f);
	light->SetIntensity(10.0f);
	//
	//scene->AddInstance(model);
	//scene->SetMainCamera(camera);
	//scene->AddInstance(camera);
	scene->AddInstance(environmentLight);
	scene->AddInstance(light);

	//CModel* animModel = CModelFactory::GetInstance()->LoadModelPBR("Ani/CH_NPC_Undead_17G3_SK.fbx");
	//
	//CAnimation* animation = new CAnimation();
	//std::vector<std::string> somePathsToAnimations;
	//somePathsToAnimations.push_back("ani/CH_NPC_Undead@Walk_01_17G3_AN.fbx");
	//somePathsToAnimations.push_back("ani/CH_NPC_Undead@Idle_01_17G3_AN.fbx");
	//
	//const std::string rigModel = "Ani/CH_NPC_Undead_17G3_SK.fbx";
	//animation->Init(rigModel.c_str(), somePathsToAnimations);
	//animModel->AddAnimation(animation);
	//
	//CModelInstance* animModelInstance = new CModelInstance();
	//animModelInstance->Init(animModel);
	//animModelInstance->SetScale(0.025f);
	//animModelInstance->SetBlend(0, 1, 1.0f);
	//animModelInstance->SetPosition({ 10.0f, 0.0f, 10.0f });
	//scene->AddInstance(animModelInstance);
}

void CGame::Update()
{
	//auto models = CScene::GetInstance()->CullModels(CScene::GetInstance()->GetMainCamera());
	//for (auto& model : models)
	//{
	//	model->SetBlend(0, 1, sinf(CTimer::Time()));
	//	model->UpdateAnimation(CTimer::Dt());//CTimer::Dt());
	//}


	UpdateCamera();

	if (Input::GetInstance()->IsKeyPressed('1')) {
		myLevelLoader->LoadNewLevel("Levels/SampleScene_exportedLevelASCII.txt");
	}
	if (Input::GetInstance()->IsKeyPressed('2')) {
		myLevelLoader->LoadNewLevel("Levels/SampleScene_exportedLevelASCII_Smily.txt");
	}
	UpdatePointLights();
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

void CGame::UpdatePointLights()
{
	float lightMoveSpeed = 5.0f;
	CPointLight* light = CScene::GetInstance()->GetPointLights()[0];
	SM::Vector3 position = light->GetPosition();
	if (Input::GetInstance()->IsKeyDown(VK_LEFT))
	{
		position.x -= lightMoveSpeed * CTimer::Dt();
	}
	if (Input::GetInstance()->IsKeyDown(VK_RIGHT))
	{
		position.x += lightMoveSpeed * CTimer::Dt();
	}
	if (Input::GetInstance()->IsKeyDown(VK_UP))
	{
		position.y += lightMoveSpeed * CTimer::Dt();
	}
	if (Input::GetInstance()->IsKeyDown(VK_DOWN))
	{
		position.y -= lightMoveSpeed * CTimer::Dt();
	}
	light->SetPosition(position);

	if (Input::GetInstance()->IsKeyPressed(VK_RETURN))
	{
		CCamera* camera = CScene::GetInstance()->GetMainCamera();
		//camera->SetPosition(position);
		camera->SetTransform(light->GetPosition(), { 0.0f, 30.0f, 0.0f });
	}
}


#pragma region Measure Time diff between Static Timer/caching float with dt
//std::string printTxt = "";
//auto start = high_resolution_clock::now();
////NonStatic Call
//static float totalDuration = 0.0f;
//totalDuration += dt * 3.0f;	
//CScene::GetInstance()->GetEnvironmentLight()->SetDirection({ sinf(totalDuration), 0.0f, cosf(totalDuration) });
////Nonstatic Call
//auto stop = high_resolution_clock::now();
//auto duration = duration_cast<nanoseconds>(stop - start).count();
//printTxt.append("NonStatic: ");
//printTxt += std::to_string(duration);
//printTxt.append("\n");
//OutputDebugStringA(printTxt.c_str());


//std::string printTxt = "";
//auto start = high_resolution_clock::now();
////Static Call
//CScene::GetInstance()->GetEnvironmentLight()->SetDirection({ sinf(CTimer::Time()), 0.0f, cosf(CTimer::Time()) });
////Static Call
//auto stop = high_resolution_clock::now();
//auto duration = duration_cast<nanoseconds>(stop - start).count();
//printTxt.append("Static: ");
//printTxt += std::to_string(duration);
//printTxt.append("\n");
//OutputDebugStringA(printTxt.c_str());

//std::string printTxt = "";
//auto start = high_resolution_clock::now();
////Static Call
////for (int i = 0; i < 100; ++i)
////{
//	CScene::GetInstance()->GetEnvironmentLight()->SetDirection({ sinf(CTimer::Time()), 0.0f, cosf(CTimer::Time()) });
////}
////Static Call
//auto stop = high_resolution_clock::now();
//auto duration = duration_cast<nanoseconds>(stop - start).count();
//printTxt.append("Static: ");
//printTxt += std::to_string(duration);
//printTxt.append("\n");
//OutputDebugStringA(printTxt.c_str());

//std::string printTxt = "";
//auto start = high_resolution_clock::now();
////Cache Static Call
//const float staticTotalDuration = CTimer::Time();
//CScene::GetInstance()->GetEnvironmentLight()->SetDirection({ sinf(staticTotalDuration), 0.0f, cosf(staticTotalDuration) });
////Cache Static Call
//auto stop = high_resolution_clock::now();
//auto duration = duration_cast<nanoseconds>(stop - start).count();
//printTxt.append("Cache Static: ");
//printTxt += std::to_string(duration);
//printTxt.append("\n");
//OutputDebugStringA(printTxt.c_str());

//std::vector<CModelInstance*> models = CScene::GetInstance()->CullModels(nullptr);
//for (auto& model : models)
//{
//	model->Rotate({ 0, dt, 0 });
//}
#pragma endregion