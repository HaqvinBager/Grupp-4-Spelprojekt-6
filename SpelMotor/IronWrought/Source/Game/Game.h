#pragma once

namespace DirectX {
	namespace SimpleMath {
		struct Vector3;
	}
}
class CLevelLoader;
class CModelInstance;
class CShowCase;
class CGame
{
public:
	CGame();
	~CGame();

	void Init();
	void Update();


private:

	//void UpdateCamera();
	//void CapsuleColliderTest();

	CLevelLoader* myLevelLoader;
	CShowCase* myShowCase;
};

