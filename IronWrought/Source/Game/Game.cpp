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
	scene->AddInstance(CreateModel(SM::Vector3(4.1f, -2.5f, 7.0f)));

	CLightFactory* lightFactory = CLightFactory::GetInstance();
	CEnvironmentLight* environmentLight = lightFactory->CreateEnvironmentLight("CubeMapTexture.dds");
	environmentLight->SetDirection(SM::Vector3(0, 1, -1));
	environmentLight->SetColor(SM::Vector3(0.8f, 0.8f, 0.8f));
	scene->AddInstance(environmentLight);
}

void CGame::Update(const float dt)
{
	UpdateCamera(dt);
	UpdateScene(dt);
}

void CGame::UpdateCamera(const float& dt)
{
	float cameraMoveSpeed = 10.0f;
	SM::Vector3 camera_movement_input(0, 0, 0);
	camera_movement_input.z = Input::GetInstance()->IsKeyDown('W') ? cameraMoveSpeed : camera_movement_input.z;
	camera_movement_input.z = Input::GetInstance()->IsKeyDown('S') ? -cameraMoveSpeed : camera_movement_input.z;
	camera_movement_input.x = Input::GetInstance()->IsKeyDown('D') ? cameraMoveSpeed : camera_movement_input.x;
	camera_movement_input.x = Input::GetInstance()->IsKeyDown('A') ? -cameraMoveSpeed : camera_movement_input.x;
	camera_movement_input.y = Input::GetInstance()->IsKeyDown('E') ? cameraMoveSpeed : camera_movement_input.y;
	camera_movement_input.y = Input::GetInstance()->IsKeyDown('Q') ? -cameraMoveSpeed : camera_movement_input.y;

	float rotationSpeed = 1.5f;
	float camera_rotation_input = 0;
	camera_rotation_input = Input::GetInstance()->IsKeyDown('R') ? -rotationSpeed : camera_rotation_input;
	camera_rotation_input = Input::GetInstance()->IsKeyDown('F') ? rotationSpeed : camera_rotation_input;
	
	auto camera = CScene::GetInstance()->GetMainCamera();
	camera->Move(camera_movement_input * dt);
	camera->Rotate({ 0, camera_rotation_input * dt, 0 });
}

void CGame::UpdateScene(const float& dt)
{
	std::vector<CModelInstance*> models = CScene::GetInstance()->CullModels(nullptr);
	for (auto& model : models)
	{
		model->Rotate({ 0, dt, 0 });
	}
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