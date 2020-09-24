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

	void UpdatePointLights();
	void UpdateCamera();
};

