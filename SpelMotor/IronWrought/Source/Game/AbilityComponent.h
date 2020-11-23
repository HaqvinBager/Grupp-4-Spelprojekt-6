#pragma once
#include "Behaviour.h"
#include "InputObserver.h"

class CGameObject;

enum class EAbilityType {
	//TEMP TYPES TODO: CHANGE TO PROPER NAMES
	WHIRLWIND,
	TRIANGLE,
	BOX
};

class CAbilityComponent : public CBehaviour, public IInputObserver
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

	void SendEvent();
	void RecieveEvent(const EInputEvent aEvent) override;

private:
	CGameObject* LoadAbilityFromFile(EAbilityType anAbilityType);

private:
	std::map<EAbilityType, std::vector<CGameObject*>> myAbilityPools;
	std::vector<CGameObject*> myActiveAbilities;
	float* myCurrentCooldowns;
	float* myMaxCooldowns;
};
