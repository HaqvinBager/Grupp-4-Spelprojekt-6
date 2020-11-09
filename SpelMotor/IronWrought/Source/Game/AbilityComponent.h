#pragma once
#include "Component.h"

class CGameObject;

enum class EAbilityType {
	WHIRLWIND
};

class CAbilityComponent : public CComponent
{
public:
	CAbilityComponent(CGameObject& aParent, std::vector<std::pair<EAbilityType, unsigned int>> someAbilities);
	~CAbilityComponent();

	void Awake() override;
	void Start() override;
	void Update() override;

	void UseAbility(EAbilityType anAbilityType, DirectX::SimpleMath::Vector3 aSpawnPosition);

private:
	CGameObject* LoadAbilityFromFile(EAbilityType anAbilityType);

private:
	std::map<EAbilityType, std::vector<CGameObject*>> myAbilityPools;
	std::vector<CGameObject*> myActiveAbilities;
};

