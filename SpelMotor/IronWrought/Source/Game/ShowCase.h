#pragma once
#include <SimpleMath.h>

using namespace DirectX::SimpleMath;
class CLevelLoader;
class CGameObject;
class CCamera;

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
	void UpdatePlayerController();

private:
	CGameObject* myPlayer;
	CGameObject* myEnemy;
	CCamera* myCamera;
	CCamera* myFreeCamera;
	CLevelLoader* myLevelLoader;
	SNavMesh* myNavMesh;
	std::vector<DirectX::SimpleMath::Vector3> myPlayerPath;
};
