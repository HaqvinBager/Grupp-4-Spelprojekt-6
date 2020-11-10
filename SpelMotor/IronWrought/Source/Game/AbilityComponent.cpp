#include "stdafx.h"
#include "AbilityComponent.h"
#include "GameObject.h"
#include "VFXComponent.h"
#include "VFXFactory.h"
#include "ParticleFactory.h"
#include "ParticleEmitterComponent.h"
#include "AbilityBehaviorComponent.h"
#include "ProjectileBehavior.h"
#include "TransformComponent.h"
#include "Scene.h"

CAbilityComponent::CAbilityComponent(CGameObject& aParent, std::vector<std::pair<EAbilityType, unsigned int>> someAbilities) : CComponent(aParent)
{
	// Setting up pools
	for (unsigned int i = 0; i < someAbilities.size(); ++i) {
		std::vector<CGameObject*> gameObjectsToPool;
		for (unsigned int j = 0; j < someAbilities[i].second; ++j) {
			gameObjectsToPool.emplace_back(LoadAbilityFromFile(someAbilities[i].first));
		}
		myAbilityPools.emplace(someAbilities[i].first, gameObjectsToPool);
	}
}

CAbilityComponent::~CAbilityComponent()
{
}

void CAbilityComponent::Awake()
{
}

void CAbilityComponent::Start()
{
}

void CAbilityComponent::Update()
{
	for (unsigned int i = 0; i < myActiveAbilities.size(); ++i) {
		if (!myActiveAbilities[i]->Active()) {
			std::swap(myActiveAbilities[i], myActiveAbilities.back());
			myAbilityPools.at(myActiveAbilities.back()->GetComponent<CAbilityBehaviorComponent>()->GetAbilityType()).emplace_back(std::move(myActiveAbilities.back()));
			myActiveAbilities.pop_back();
		}
	}
}

void CAbilityComponent::UseAbility(EAbilityType anAbilityType, DirectX::SimpleMath::Vector3 aSpawnPosition)
{
	if (myAbilityPools.at(anAbilityType).empty()) {
		return;
	}

	myActiveAbilities.emplace_back(myAbilityPools.at(anAbilityType).back());
	myAbilityPools.at(anAbilityType).pop_back();
	myActiveAbilities.back()->Active(true);
	myActiveAbilities.back()->myTransform->Position(aSpawnPosition);

	if (anAbilityType == EAbilityType::WHIRLWIND) {
		myActiveAbilities.back()->GetComponent<CAbilityBehaviorComponent>()->Init(aSpawnPosition);
	}
}

CGameObject* CAbilityComponent::LoadAbilityFromFile(EAbilityType anAbilityType)
{
	CGameObject* abilityTest = new CGameObject();
	CProjectileBehavior* behavior;
	DirectX::SimpleMath::Vector3 abilityDirection;
	switch (anAbilityType)
	{
	case EAbilityType::WHIRLWIND:
		abilityTest->myTransform->Position({ 0.0f, 0.0f, 0.0f });
		abilityTest->AddComponent<CVFXComponent>(*abilityTest);
		abilityTest->GetComponent<CVFXComponent>()->Init(CVFXFactory::GetInstance()->GetVFXBase("Assets/3D/VFX/VFX_mesh_disc_01_19G4.fbx", "VFXData_FogWall.json"));

		abilityTest->AddComponent<CParticleEmitterComponent>(*abilityTest);
		abilityTest->GetComponent<CParticleEmitterComponent>()->Init(CParticleFactory::GetInstance()->GetParticle("ParticleData_SmokeEmitter.json"));

		abilityDirection = { 0.0f, 0.0f, 0.0f };

		behavior = new CProjectileBehavior(abilityDirection, 3.0f);
		abilityTest->AddComponent<CAbilityBehaviorComponent>(*abilityTest, behavior, EAbilityType::WHIRLWIND);

		abilityTest->Active(false);
		CScene::GetInstance()->AddInstance(abilityTest);
		break;
	default:
		break;
	}
	return abilityTest;
}
