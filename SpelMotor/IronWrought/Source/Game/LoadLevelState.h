#pragma once
#include "State.h"
#include <future>
#include "SceneReader.h"
#include "UnityFactory.h"

enum class ELevel
{
	LoadScreen,
	Dungeon,
	Gardens,
	Castle
};


class CLoadLevelState : public CState {

public:
	CLoadLevelState(CStateStack& aStateStack);
	~CLoadLevelState() override;

	void Awake() override;
	void Start() override;
	void Update() override;
private:

	/*void InitLoadScreen();
	void LoadInGameData();*/

	unsigned int Load(const ELevel aLevel);

	void SaveLevelNames();
	void SaveModelPaths(const ELevel aLevel);
	std::string& BinPath(const ELevel aLevel);
	std::vector<std::string>& BinModelPaths(const ELevel aLevel);

	CSceneReader mySceneReader;
	CUnityFactory myUnityFactory;

	std::future<unsigned int> myLoadLevelFuture;
	std::vector<std::string> myLevelNames;
	std::vector<std::string> myBinPaths;
	std::unordered_map<ELevel, std::string> myModelJsonFileMap;
	std::unordered_map<ELevel, std::vector<std::string>> myBinModelPaths;
};


