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

#include <chrono> 
using namespace std::chrono;

#ifdef _DEBUG
#pragma comment(lib, "Engine_Debug.lib")
#endif // _DEBUG
#ifdef NDEBUG
#pragma comment(lib, "Engine_Release.lib")
#endif // NDEBUG




namespace SM = DirectX::SimpleMath;
using namespace CommonUtilities;

CGame::CGame() : myEnemyModel(nullptr)
{
}

CGame::~CGame()
{
}

void CGame::Init()
{
	CCamera* camera = CCameraFactory::GetInstance()->CreateCamera(65.0f);
	camera->SetTransform(SM::Vector3(0.0f, 0.0f, 0.0f), SM::Vector3(0.0f, 0.0f, 0.0f));
	camera->SetPosition(SM::Vector3(0, 0, 0.0f));

	CScene* scene = CScene::GetInstance();
	scene->AddInstance(camera);
	scene->SetMainCamera(camera);
	scene->AddInstance(CreateModel(SM::Vector3(-4.0f, -2.5f, 7.0f)));
	scene->AddInstance(CreateModel(SM::Vector3(0.0f, -2.5f, 7.0f)));
	scene->AddInstance(CreateModel(SM::Vector3(4.0f, -2.5f, 7.0f)));

	CLightFactory* lightFactory = CLightFactory::GetInstance();
	CEnvironmentLight* environmentLight = lightFactory->CreateEnvironmentLight("Yokohama2.dds");
	environmentLight->SetDirection(SM::Vector3(0, 1, -1));
	environmentLight->SetColor(SM::Vector3(0.8f, 0.8f, 0.8f));
	scene->AddInstance(environmentLight);
}

void CGame::Update()
{
	UpdateCamera();
	UpdateScene();
}

void CGame::UpdateCamera()
{
	const float dt = CTimer::Dt();
	float cameraMoveSpeed = 10.0f;
	float verticalMoveSpeedModifier = 0.5f;
	SM::Vector3 camera_movement_input(0, 0, 0);
	camera_movement_input.z = Input::GetInstance()->IsKeyDown('W') ? cameraMoveSpeed : camera_movement_input.z;
	camera_movement_input.z = Input::GetInstance()->IsKeyDown('S') ? -cameraMoveSpeed : camera_movement_input.z;
	camera_movement_input.x = Input::GetInstance()->IsKeyDown('D') ? cameraMoveSpeed : camera_movement_input.x;
	camera_movement_input.x = Input::GetInstance()->IsKeyDown('A') ? -cameraMoveSpeed : camera_movement_input.x;
	camera_movement_input.y = Input::GetInstance()->IsKeyDown('R') ? cameraMoveSpeed * verticalMoveSpeedModifier : camera_movement_input.y;
	camera_movement_input.y = Input::GetInstance()->IsKeyDown('F') ? -cameraMoveSpeed * verticalMoveSpeedModifier : camera_movement_input.y;

	float rotationSpeed = 1.5f;
	float camera_rotation_input = 0;
	camera_rotation_input = Input::GetInstance()->IsKeyDown('Q') ? rotationSpeed : camera_rotation_input;
	camera_rotation_input = Input::GetInstance()->IsKeyDown('E') ? -rotationSpeed : camera_rotation_input;
	
	auto camera = CScene::GetInstance()->GetMainCamera();
	camera->Move(camera_movement_input * dt);
	camera->Rotate({ 0, camera_rotation_input * dt, 0 });	
}

void CGame::UpdateScene()
{
	const float time = CTimer::Time();
	CScene::GetInstance()->GetEnvironmentLight()->SetDirection({ sinf(time), 0.0f, cosf(time) });
}

CModelInstance* CGame::CreateModel(SM::Vector3 position)
{
	CModel* model = CModelFactory::GetInstance()->GetModel("Model/enemy_robot/enemy_robot.fbx");
	//CModel* model = CModelFactory::GetInstance()->GetModel("Model/hejsanhopsan/hejsanhopsan.fbx");
	myEnemyModel = new CModelInstance();
	myEnemyModel->Init(model);
	myEnemyModel->SetPosition(position);
	return myEnemyModel;
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