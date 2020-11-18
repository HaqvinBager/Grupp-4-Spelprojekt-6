#include "stdafx.h"
#include "LoadLevelState.h"
#include "InGameState.h"
#include "StateStack.h"
#include "LevelLoader.h"
#include <CUnityLoader.h> //In order to know the size of ObjectData
#include "Input.h"
#include "Scene.h"
#include "Engine.h"

using namespace CommonUtilities;

CLoadLevelState::CLoadLevelState(CStateStack& aStateStack) : CState(aStateStack)
{

}

CLoadLevelState::~CLoadLevelState()
{

}

void CLoadLevelState::Awake()
{
	//CScene::GetInstance()->Ready(false);
	myLevelLoader.Init(0);
	CEngine::GetInstance()->SetActiveScene(0);
}

void CLoadLevelState::Start()
{
	myLoadLevelFuture = std::async(std::launch::async, &CLevelLoader::Init, myLevelLoader, 1);
}

void CLoadLevelState::Update()
{
	if (myLoadLevelFuture._Is_ready())
	{
		if (Input::GetInstance()->IsKeyPressed('K'))
		{
			CEngine::GetInstance()->SetActiveScene(1);
			CScene::GetInstance()->Ready(myLoadLevelFuture.get());
			myStateStack.PushState(new CInGameState(myStateStack));
			myStateStack.Awake();
			myStateStack.Start();
		}
	}
}