#pragma once
#include <Component.h>

class CGameObject;
class IAbilityBehavior;

class CAbilityBehaviorComponent : public CComponent
{
public:
	CAbilityBehaviorComponent(CGameObject& aParent, IAbilityBehavior* aBehavior);
	~CAbilityBehaviorComponent() override;

	void Awake() override;
	void Start() override;
	void Update() override;

private:
	IAbilityBehavior* myBehavior;
};

