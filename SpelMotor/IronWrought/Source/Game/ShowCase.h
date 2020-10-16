#pragma once
#include <SimpleMath.h>

using namespace DirectX::SimpleMath;
class CLevelLoader;
class CGameObject;
class CCamera;
class CShowCase
{
public:
	CShowCase();
	~CShowCase();

	void Init();
	void Update();

private:
	CGameObject* CreatePlayer(Vector3 aPosition);
	CGameObject* CreateEnemy(Vector3 aPosition);
	CCamera* CreateCamera(CGameObject* aCameraTarget);
	void UpdatePlayerController();

private:
	CGameObject* myPlayer;
	CGameObject* myEnemy;
	CCamera* myCamera;
	CLevelLoader* myLevelLoader;
};
