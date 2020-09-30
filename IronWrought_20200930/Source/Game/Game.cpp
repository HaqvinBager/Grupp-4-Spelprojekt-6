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

#include "CUnityLoader.h"

#ifdef _DEBUG
#pragma comment(lib, "Engine_Debug.lib")
#pragma comment(lib, "UnityImporterApp_Debug.lib")
#endif // _DEBUG
#ifdef NDEBUG
#pragma comment(lib, "Engine_Release.lib")
#endif // NDEBUG

namespace SM = DirectX::SimpleMath;
using namespace CommonUtilities;

#define UNITY_SCALE_MODIFIER 1

CGame::CGame()
	: myEnemyModel(nullptr)
	, myUnityLoader(new CUnityLoader())
{
}

CGame::~CGame()
{
}

void CGame::Init()
{
	CCamera* camera = CCameraFactory::GetInstance()->CreateCamera(65.0f);
	camera->SetTransform(SM::Vector3(0.0f, 0.0f, -50.0f), SM::Vector3(0.0f, 0.0f, 0.0f));
	camera->SetPosition(SM::Vector3(0, 0, 0.0f));

	CScene* scene = CScene::GetInstance();
	scene->AddInstance(camera);
	scene->SetMainCamera(camera);
	//scene->AddInstance(CreateModel(SM::Vector3(-4.0f, -2.5f, 7.0f)));
	//scene->AddInstance(CreateModel(SM::Vector3(0.0f, -2.5f, 7.0f)));
	//scene->AddInstance(CreateModel(SM::Vector3(0.55f, -2.5f, 7.0f)));
	
	//LoadUnityScene("Unity Level Export/level1.txt");// HEADS - works
	//LoadUnityScene("Unity Level Export/exportedLevelASCII.txt");// Robot sin-wave - works
	LoadUnityScene("Unity Level Export/SampleScene_exportedLevelASCII.txt");
	/*
		FBX scales are very important!.
		Positions are inverted?
	*/

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
	camera_movement_input.y = Input::GetInstance()->IsKeyDown(VK_SPACE) ? cameraMoveSpeed : camera_movement_input.y;
	camera_movement_input.y = Input::GetInstance()->IsKeyDown('C') ? -cameraMoveSpeed : camera_movement_input.y;

	float rotationSpeed = 1.5f;
	float camera_rotation_input = 0;
	float camera_rotation_input_vertical = 0;
	camera_rotation_input = Input::GetInstance()->IsKeyDown('E') ? -rotationSpeed : camera_rotation_input;
	camera_rotation_input = Input::GetInstance()->IsKeyDown('Q') ? rotationSpeed : camera_rotation_input;
	camera_rotation_input_vertical = Input::GetInstance()->IsKeyDown('Z') ? -rotationSpeed : camera_rotation_input_vertical;
	camera_rotation_input_vertical = Input::GetInstance()->IsKeyDown('X') ? rotationSpeed : camera_rotation_input_vertical;
	
	auto camera = CScene::GetInstance()->GetMainCamera();
	camera->Move(camera_movement_input * dt);
	camera->Rotate({ camera_rotation_input_vertical * dt, camera_rotation_input * dt, 0 });
}

void CGame::LoadUnityScene(const std::string& aFilePath)
{
	std::vector<ObjectData> objectData;
#if _DEBUG
	CScene* scene = CScene::GetInstance();
	std::string modelPath = aFilePath;
	myUnityLoader->LoadModels(modelPath);
	objectData = myUnityLoader->LoadGameObjects(modelPath, EReadMode::EReadMode_ASCII);
	for (auto& object : objectData)
	{
		if (object.myRelativePath.length() > 1)
		{
			CModelInstance* model = CreateModel(object.myRelativePath, SM::Vector3(object.myPosX, object.myPosY, object.myPosZ), SM::Vector3(DirectX::XMConvertToRadians(object.myRotX), DirectX::XMConvertToRadians(object.myRotY), DirectX::XMConvertToRadians(object.myRotZ)));
			//CModelInstance* model = CreateModel(object.myRelativePath, SM::Vector3(object.myPosX, object.myPosY, object.myPosZ), SM::Vector3(object.myRotX, object.myRotY, object.myRotZ));
			//model->SetScale(SM::Vector3(0.1f,0.1f,0.1f));
			scene->AddInstance(model);
		}
	}

#endif // _DEBUG
#if NDEBUG
	objectData = myUnityLoader->LoadGameObjects(aFilePath, EReadMode::EReadMode_Binary);
#endif // NDEBUG

	
}

void CGame::UpdateScene(const float& dt)
{
	if (Input::GetInstance()->IsKeyDown('U'))
	{
		std::vector<CModelInstance*> models = CScene::GetInstance()->CullModels(nullptr);
		for (auto& model : models)
		{
			model->Rotate({ 0, dt, 0 });
		}
	}
	if (Input::GetInstance()->IsKeyDown('J'))
	{
		std::vector<CModelInstance*> models = CScene::GetInstance()->CullModels(nullptr);
		for (auto& model : models)
		{
			model->Rotate({ 0, 0, dt });
		}
	}

}

CModelInstance* CGame::CreateModel(const std::string& aModelPath, SM::Vector3 position, SM::Vector3 aRotation)
{
	CModel* model = CModelFactory::GetInstance()->GetModel(aModelPath);
	//CModel* model = CModelFactory::GetInstance()->GetModel("enemy_robot.fbx");
	//CModel* model = CModelFactory::GetInstance()->GetModel("Model/hejsanhopsan/hejsanhopsan.fbx");
	myEnemyModel = new CModelInstance();
	myEnemyModel->Init(model);
	myEnemyModel->SetTransform(position, aRotation, SM::Vector3(0.01f, 0.01f, 0.01f));
	return myEnemyModel;
}