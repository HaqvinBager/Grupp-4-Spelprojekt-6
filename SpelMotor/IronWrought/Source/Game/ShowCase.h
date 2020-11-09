#pragma once
#include <SimpleMath.h>

using namespace DirectX::SimpleMath;
class CLevelLoader;
class CGameObject;
class CCamera; 
class CDialogueSystem;
class CObjectPool;
class CTokenPool;

struct SNavMesh;

class CShowCase
{
public:
	CShowCase();
	~CShowCase();

	void Init();
	void Update();

	unsigned int myWindowWidth;
	unsigned int myWindowHeight;

private:
	CGameObject* CreatePlayer(Vector3 aPosition);
	CGameObject* CreateEnemy(Vector3 aPosition);
	CCamera* CreateCamera(CGameObject* aCameraTarget);
	void CreateAbility(Vector3 aPosition);
	void UpdatePlayerController();

private:
	CGameObject* myPlayer;
	std::vector<CGameObject*> myEnemies;
	CCamera* myCamera;
	CCamera* myFreeCamera;
	CLevelLoader* myLevelLoader;
	SNavMesh* myNavMesh;
	std::vector<DirectX::SimpleMath::Vector3> myPlayerPath;
	class CStateStack* myStateStack;
	class CMenuState* myMenuState;
	class CInGameState* myInGameState;
	CDialogueSystem* myDialogueSystem;
	CObjectPool* myEnemyPool;
	CTokenPool* myTokenPool;
};
