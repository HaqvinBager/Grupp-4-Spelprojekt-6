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

CAbilityComponent::CAbilityComponent(CGameObject& aParent, std::vector<std::pair<EAbilityType, int>> someAbilities) : CComponent(aParent)
{
	// Setting up pools
	//for (unsigned int i = 0; i < someAbilities.size(); ++i) {
	//	std::vector<CGameObject*> gameObjectsToPool;
	//	for (unsigned int i = 0; i < someAbilities[i].second; ++i) {
	//		gameObjectsToPool.emplace_back(LoadAbilityFromFile(someAbilities[i].first));
	//	}
	//	myAbilityPools.emplace(someAbilities[i].first, gameObjectsToPool);
	//}
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
	//for (unsigned int i = 0; i < myActiveAbilities.size(); ++i) {
	//	if (!myActiveAbilities[i]->Enabled()) {
	//		std::swap(myActiveAbilities[i], myActiveAbilities.back());
	//		my
	//	}
	//}
}

void CAbilityComponent::UseAbility(EAbilityType anAbilityType, DirectX::SimpleMath::Vector3 aSpawnPosition)
{
	//myActiveAbilities.emplace_back(myAbilityPools.at(anAbilityType).back());
	//myAbilityPools.at(anAbilityType).pop_back();
	//myActiveAbilities.back()->Enabled(true);
	myAbilityPools.at(anAbilityType).back()->Enabled(true);
	myAbilityPools.at(anAbilityType).back()->myTransform->Position(aSpawnPosition);
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
		abilityTest->GetComponent<CVFXComponent>()->Init(CVFXFactory::GetInstance()->GetVFXBase("Assets/3D/VFX/Disc_test.fbx", "VFXData_FogWall.json"));

		abilityTest->AddComponent<CParticleEmitterComponent>(*abilityTest);
		abilityTest->GetComponent<CParticleEmitterComponent>()->Init(CParticleFactory::GetInstance()->GetParticle("ParticleData_SmokeEmitter.json"));

		abilityDirection = { 1.0f, 0.0f, 0.0f };

		behavior = new CProjectileBehavior(abilityDirection, 3.0f);
		abilityTest->AddComponent<CAbilityBehaviorComponent>(*abilityTest, behavior);
		CScene::GetInstance()->AddInstance(abilityTest);
		break;
	default:
		break;
	}
	return abilityTest;
}
