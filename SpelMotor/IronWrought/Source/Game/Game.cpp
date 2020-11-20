#include "stdafx.h"
#include "Game.h"
#include "LoadLevelState.h"

#include <SceneReader.h>

#ifdef _DEBUG
#pragma comment(lib, "Engine_Debug.lib")
#pragma comment(lib, "UnityImporterApp_Debug.lib")
#endif // _DEBUG
#ifdef NDEBUG
#pragma comment(lib, "Engine_Release.lib")
#pragma comment(lib, "UnityImporterApp_Release.lib")
#endif // NDEBUG

CGame::CGame() 
	: myWindowWidth(0)
	, myWindowHeight(0)
{
}

CGame::~CGame()
{
}

void CGame::Init()
{
	myStateStack.PushState(new CLoadLevelState(myStateStack));
	myStateStack.Awake();
	myStateStack.Start();
}

void CGame::Update()
{
	myStateStack.Update();
}
	
