#pragma once
#include <Component.h>
#include "AbilityComponent.h"

class CGameObject;
class IAbilityBehavior;

class CAbilityBehaviorComponent : public CComponent
{
public:
	CAbilityBehaviorComponent(CGameObject& aParent, IAbilityBehavior* aBehavior, EAbilityType anAbilityType);
	~CAbilityBehaviorComponent() override;

	void Awake() override;
	void Start() override;
	void Update() override;

	EAbilityType GetAbilityType() const;

	void Init(DirectX::SimpleMath::Vector3 aCasterPosition);

private:
	IAbilityBehavior* myBehavior;
	EAbilityType myAbilityType;
};
