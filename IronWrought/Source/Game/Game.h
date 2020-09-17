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
	void Update(const float dt);

private:

	void UpdateScene(const float& dt);
	void UpdateCamera(const float& dt);
	CModelInstance* CreateModel(DirectX::SimpleMath::Vector3 position);
	CModelInstance* myEnemyModel;
};

