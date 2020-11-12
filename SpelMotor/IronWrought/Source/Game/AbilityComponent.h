#pragma once
#include "Behaviour.h"

class CGameObject;

enum class EAbilityType {
	//TEMP TYPES TODO: CHANGE TO PROPER NAMES
	WHIRLWIND,
	TRIANGLE,
	BOX
};

class CAbilityComponent : public CBehaviour
{
public:
	CAbilityComponent(CGameObject& aParent, std::vector<std::pair<EAbilityType, unsigned int>> someAbilities);
	~CAbilityComponent();

	void Awake() override;
	void Start() override;
	void Update() override;

	void OnEnable() override;
	void OnDisable() override;

	void UseAbility(EAbilityType anAbilityType, DirectX::SimpleMath::Vector3 aSpawnPosition);

private:
	CGameObject* LoadAbilityFromFile(EAbilityType anAbilityType);

private:
	std::map<EAbilityType, std::vector<CGameObject*>> myAbilityPools;
	std::vector<CGameObject*> myActiveAbilities;
};

