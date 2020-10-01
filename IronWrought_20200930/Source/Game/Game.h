#pragma once
#include <string>
class CUnityLoader;

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

	void LoadUnityScene(const std::string& aFilePath);
	void UpdateScene(const float& dt);
	void UpdateCamera(const float& dt);
	CModelInstance* CreateModel(const std::string& aModelPath, DirectX::SimpleMath::Vector3 position, DirectX::SimpleMath::Vector3 aRotation);
	CModelInstance* myEnemyModel;
	CUnityLoader*   myUnityLoader;
};

