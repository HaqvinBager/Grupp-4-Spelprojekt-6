#pragma once

namespace DirectX {
	namespace SimpleMath {
		struct Vector3;
	}
}

class CModelInstance;
class CGame
{
public:
	CGame();
	~CGame();

	void Init();
	void Update();

private:

	void UpdateScene();
	void UpdateCamera();
	CModelInstance* CreateModel(DirectX::SimpleMath::Vector3 position);
	CModelInstance* myEnemyModel;
};

