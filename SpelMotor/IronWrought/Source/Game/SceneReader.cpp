#include "stdafx.h"
#include "SceneReader.h"

CSceneReader::CSceneReader()
{
}

CSceneReader::~CSceneReader()
{
}

bool CSceneReader::OpenBin(const std::string& aBinFilePath)
{
	myStream.open(aBinFilePath, std::ios::binary);
	if (!myStream.is_open())
	{
		return false;
	}

    return true;
}

SInGameData& CSceneReader::ReadInGameData()
{
	std::string binaryData((std::istreambuf_iterator<char>(myStream)), std::istreambuf_iterator<char>());
	myStreamPtr = &binaryData[0];

	myInGameData.emplace_back(new SInGameData());

	SCameraData cameraData = {};
	myStreamPtr += Read(cameraData);
	myInGameData.back()->myCamera = cameraData;

	SDirectionalLightData directionalLightData = {};
	myStreamPtr += Read(directionalLightData);
	myInGameData.back()->myDirectionalLight = directionalLightData;

	int pointLightDataCount = 0;
	myStreamPtr += Read(pointLightDataCount);
	myInGameData.back()->myPointLightData.reserve(pointLightDataCount);
	for (int i = 0; i < pointLightDataCount; ++i)
	{
		SPointLightData pointLightData = {};
		myStreamPtr += Read(pointLightData);
		myInGameData.back()->myPointLightData.emplace_back(pointLightData);
	}

	int playerIsInScene = 0;
	myStreamPtr += Read(playerIsInScene);
	if (playerIsInScene == 1)
	{
		SPlayerData playerData = {};
		myStreamPtr += Read(playerData);
		myInGameData.back()->myPlayerData = playerData;
	}

	int eventDataCount = 0;
	myStreamPtr += Read(eventDataCount);
	myInGameData.back()->myEventData.reserve(eventDataCount);
	for (int i = 0; i < eventDataCount; ++i) {
		SEventData evenData = {};
		myStreamPtr += Read(evenData);
		myInGameData.back()->myEventData.emplace_back(evenData);
	}

	int enemyDataCount = 0;
	myStreamPtr += Read(enemyDataCount);
	myInGameData.back()->myEnemyData.reserve(enemyDataCount);
	for (int i = 0; i < enemyDataCount; ++i) {
		SEnemyData enemyData = {};
		myStreamPtr += Read(enemyData);
		myInGameData.back()->myEnemyData.emplace_back(enemyData);
	}

	int myGameObjectDataCount = 0;
	myStreamPtr += Read(myGameObjectDataCount);
	myInGameData.back()->myGameObjects.reserve(myGameObjectDataCount);
	for (int i = 0; i < myGameObjectDataCount; ++i)
	{
		SGameObjectData gameObjectData = { 0 };
		myStreamPtr += Read(gameObjectData);
		myInGameData.back()->myGameObjects.emplace_back(gameObjectData);
	}

	myStream.close();
	myStreamPtr = nullptr;

	return *myInGameData.back();
}

SLoadScreenData& CSceneReader::ReadLoadScreenData()
{
	std::string binaryData((std::istreambuf_iterator<char>(myStream)), std::istreambuf_iterator<char>());
	myStreamPtr = &binaryData[0];

	myLoadScreenData.emplace_back(new SLoadScreenData());

	SCameraData cameraData = {};
	myStreamPtr += Read(cameraData);
	myLoadScreenData.back()->myCamera = cameraData;

	SDirectionalLightData directionalLightData = {};
	myStreamPtr += Read(directionalLightData);
	myLoadScreenData.back()->myDirectionalLight = directionalLightData;

	unsigned int pointLightcount = 0;
	myStreamPtr += Read(pointLightcount);

	unsigned int isPlayerInScene = 0;
	myStreamPtr += Read(isPlayerInScene);

	unsigned int eventDataCount = 0;
	myStreamPtr += Read(eventDataCount);

	unsigned int enemyDataCount = 0;
	myStreamPtr += Read(enemyDataCount);

	unsigned int gameObjectCount = 0;
	myStreamPtr += Read(gameObjectCount);
	
	SGameObjectData gameObjectData = {};
	myStreamPtr += Read(gameObjectData);
	myLoadScreenData.back()->myGameObject = gameObjectData;

	myStream.close();
	myStreamPtr = nullptr;

	return *myLoadScreenData.back();
}
