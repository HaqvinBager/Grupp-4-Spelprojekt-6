#pragma once

class CGameObject;
struct SAbilityData;

class IAbilityBehavior {
public:
	virtual ~IAbilityBehavior() = 0;

	virtual void Update(CGameObject* aParent) = 0;

	virtual void Init(DirectX::SimpleMath::Vector3 aCasterPosition) = 0;

public:
	float myDuration;
	float myTimer;
};
