#include "stdafx.h"
#include "LoadLevelState.h"
#include "InGameState.h"
#include "StateStack.h"
#include "LevelLoader.h"
#include "Input.h"
#include "Scene.h"
#include "Engine.h"
#include <iostream>
#include "JsonReader.h"
#include "GameObject.h"
#include "EnviromentLightComponent.h"
#include "EnvironmentLight.h"
#include "CameraComponent.h"
#include "Camera.h"

using namespace CommonUtilities;
using namespace rapidjson;
CLoadLevelState::CLoadLevelState(CStateStack& aStateStack) : CState(aStateStack)
{
	SaveLevelNames();
}

CLoadLevelState::~CLoadLevelState()
{

}

void CLoadLevelState::Awake()
{
	unsigned int loadSceneIndex = Load(ELevel::LoadScreen);
	CEngine::GetInstance()->SetActiveScene(loadSceneIndex);
	myLoadLevelFuture = std::async(std::launch::async, &CLoadLevelState::Load, this, ELevel::Dungeon);

	for (auto& gameObject : CEngine::GetInstance()->GetActiveScene().GetActiveGameObjects())
	{
		gameObject->Awake();
	}
}

void CLoadLevelState::Start()
{
	for (auto& gameObject : CEngine::GetInstance()->GetActiveScene().GetActiveGameObjects())
	{
		gameObject->Start();
	}
}

void CLoadLevelState::Update()
{
	if (myLoadLevelFuture._Is_ready())
	{
		CEngine::GetInstance()->SetActiveScene(myLoadLevelFuture.get());
		myStateStack.PushState(new CInGameState(myStateStack));
		myStateStack.Awake();
		myStateStack.Start();
	}

	for (auto& gameObject : CEngine::GetInstance()->GetActiveScene().GetActiveGameObjects())
	{
		gameObject->Update();
	}
}

unsigned int CLoadLevelState::Load(const ELevel aLevel)
{
	if (mySceneReader.OpenBin(BinPath(aLevel)))
	{
		SaveModelPaths(aLevel);

		if (aLevel == ELevel::LoadScreen)
		{
			SLoadScreenData& data = mySceneReader.ReadLoadScreenData();
			CScene* loadScreenScene = new CScene();
			myUnityFactory.FillScene(data, BinModelPaths(aLevel), *loadScreenScene);
			return CEngine::GetInstance()->AddScene(loadScreenScene);
		}
		else
		{
			SInGameData& data = mySceneReader.ReadInGameData();
			CScene* inGameScene = new CScene();
			myUnityFactory.FillScene(data, BinModelPaths(aLevel), *inGameScene);
			return CEngine::GetInstance()->AddScene(inGameScene);
		}
	}
	return 0;
}

void CLoadLevelState::SaveLevelNames()
{
	Document levelSelectDocument = CJsonReader::LoadDocument("Levels/LevelSelect.json");
	int count = 0;
	for (auto& levelNameObject : levelSelectDocument["Levels"].GetArray())
	{
		std::string levelBinPath = "Levels/";
		std::string levelName = levelBinPath + levelNameObject["LevelName"].GetString();
		myLevelNames.emplace_back(levelName);
		levelBinPath.append(levelNameObject["LevelName"].GetString());
		levelBinPath.append("_bin.bin");
		myBinPaths.emplace_back(levelBinPath);

		std::string modelsPath = levelName + "_bin_modelPaths.json";
		myModelJsonFileMap[static_cast<ELevel>(count)] = modelsPath;
		count++;
	}
}

void CLoadLevelState::SaveModelPaths(const ELevel aLevel)
{
	Document modelPathDocument = CJsonReader::LoadDocument(myModelJsonFileMap[aLevel]);
	for (auto& modelPath : modelPathDocument["ModelPaths"].GetArray())
	{
		myBinModelPaths[aLevel].emplace_back(modelPath["Path"].GetString());
	}
}

std::string& CLoadLevelState::BinPath(const ELevel aLevel)
{
	return myBinPaths[static_cast<int>(aLevel)];
}

std::vector<std::string>& CLoadLevelState::BinModelPaths(const ELevel aLevel)
{
	return myBinModelPaths[aLevel];
}
