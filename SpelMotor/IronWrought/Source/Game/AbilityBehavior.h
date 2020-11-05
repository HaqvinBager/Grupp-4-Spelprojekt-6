#pragma once

class CGameObject;
struct SAbilityData;

class IAbilityBehavior {
public:
	virtual ~IAbilityBehavior() = 0;

	virtual void Update(/*SAbilityData& someAbilityData, */CGameObject* aParent) = 0;
};

/* 

behaviorcomponent
abilitycomponent


add abilitycomponent to player
add behavior to ability


*/