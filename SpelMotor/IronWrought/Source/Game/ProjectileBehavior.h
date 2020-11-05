#pragma once
#include "AbilityBehavior.h"

namespace DirectX {
	namespace SimpleMath {
		struct Vector3;
	}
}

class CProjectileBehavior : public IAbilityBehavior
{
public:
	CProjectileBehavior(DirectX::SimpleMath::Vector3 aDirection, float aSpeed);
	~CProjectileBehavior() override;

	void Update(CGameObject* aParent) override;

private:
	DirectX::SimpleMath::Vector3 myDirection;
	float mySpeed;
};

