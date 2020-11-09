#pragma once

class CGameObject;
struct SAbilityData;

class IAbilityBehavior {
public:
	virtual ~IAbilityBehavior() = 0;

	virtual void Update(CGameObject* aParent) = 0;

public:
	float myDuration;
	float myTimer;
};