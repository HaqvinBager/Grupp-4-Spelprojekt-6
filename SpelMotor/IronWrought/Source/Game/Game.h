#pragma once

namespace DirectX {
	namespace SimpleMath {
		struct Vector3;
	}
}
class CLevelLoader;
class CShowCase;
class CGame
{
public:
	CGame();
	~CGame();

	void Init();
	void Update();

	unsigned int myWindowWidth;
	unsigned int myWindowHeight;

private:

	//void UpdateCamera();
	//void CapsuleColliderTest();

	CLevelLoader* myLevelLoader;
	CShowCase* myShowCase;
};

