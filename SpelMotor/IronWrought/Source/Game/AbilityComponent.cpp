#include "stdafx.h"
#include "AbilityComponent.h"
#include "GameObject.h"
#include "VFXComponent.h"
#include "VFXFactory.h"
#include "ParticleFactory.h"
#include "ParticleEmitterComponent.h"
#include "AbilityBehaviorComponent.h"
#include "TriangleColliderComponent.h"
#include "RectangleColliderComponent.h"
#include "CircleColliderComponent.h"
#include "ProjectileBehavior.h"
#include "TransformComponent.h"
#include "Scene.h"
#include "InputMapper.h"
#include "MainSingleton.h"

CAbilityComponent::CAbilityComponent(CGameObject& aParent, std::vector<std::pair<EAbilityType, unsigned int>> someAbilities) : CBehaviour(aParent)
{
	CInputMapper::GetInstance()->AddObserver(IInputObserver::EInputEvent::Ability1, this);
	CInputMapper::GetInstance()->AddObserver(IInputObserver::EInputEvent::Ability2, this);
	CInputMapper::GetInstance()->AddObserver(IInputObserver::EInputEvent::Ability3, this);


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

void CAbilityComponent::OnEnable()
{
}

void CAbilityComponent::OnDisable()
{
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

	// getparent().playanimation(myactiveabilities.back().getcomponent<pod>().myanimation);

	if (anAbilityType == EAbilityType::WHIRLWIND) {
		myActiveAbilities.back()->GetComponent<CAbilityBehaviorComponent>()->Init(aSpawnPosition);
	}
	else if (anAbilityType == EAbilityType::TRIANGLE) {
		myActiveAbilities.back()->GetComponent<CAbilityBehaviorComponent>()->Init(aSpawnPosition);
	}
	else if (anAbilityType == EAbilityType::BOX) {
		myActiveAbilities.back()->GetComponent<CAbilityBehaviorComponent>()->Init(aSpawnPosition);
	}
}

void CAbilityComponent::RecieveEvent(const EInputEvent aEvent)
{
	SMessage myMessage;
	float cooldown;
	switch (aEvent)
	{
	case EInputEvent::Ability1:
		myMessage.myMessageType = EMessageType::AbilityOneCooldown;
		cooldown = 30.0f;
		myMessage.data = &cooldown;
		CMainSingleton::PostMaster().Send(myMessage);
		break;
	case EInputEvent::Ability2:
		myMessage.myMessageType = EMessageType::AbilityTwoCooldown;
		cooldown = 30.0f;
		myMessage.data = &cooldown;
		CMainSingleton::PostMaster().Send(myMessage);
		break;
	case EInputEvent::Ability3:
		myMessage.myMessageType = EMessageType::AbilityThreeCooldown;
		cooldown = 30.0f;
		myMessage.data = &cooldown;
		CMainSingleton::PostMaster().Send(myMessage);
		break;
	default:
		break;
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
		abilityTest->GetComponent<CVFXComponent>()->Init(CVFXFactory::GetInstance()->GetVFXBase("Assets/3D/VFX/VFX_mesh_disc_01_19G4.fbx", "Json/VFXData_FogWall.json"));

		abilityTest->AddComponent<CParticleEmitterComponent>(*abilityTest);
		abilityTest->GetComponent<CParticleEmitterComponent>()->Init(CParticleFactory::GetInstance()->GetParticle("Json/ParticleData_SmokeEmitter.json"));

		abilityDirection = { 0.0f, 0.0f, 0.0f };

		behavior = new CProjectileBehavior(abilityDirection, 3.0f);
		abilityTest->AddComponent<CAbilityBehaviorComponent>(*abilityTest, behavior, EAbilityType::WHIRLWIND);

		abilityTest->AddComponent<CCircleColliderComponent>(*abilityTest, 1.0f, ECollisionLayer::PLAYERABILITY, static_cast<int>(ECollisionLayer::ENEMY));

		abilityTest->Active(false);
		CScene::GetInstance()->AddInstance(abilityTest);
		break;
	case EAbilityType::TRIANGLE:
		abilityTest->myTransform->Position({ 0.0f, 0.0f, 0.0f });
		abilityTest->AddComponent<CVFXComponent>(*abilityTest);
		abilityTest->GetComponent<CVFXComponent>()->Init(CVFXFactory::GetInstance()->GetVFXBase("Assets/3D/VFX/VFX_mesh_disc_01_19G4.fbx", "Json/VFXData_FogWall.json"));

		abilityTest->AddComponent<CParticleEmitterComponent>(*abilityTest);
		abilityTest->GetComponent<CParticleEmitterComponent>()->Init(CParticleFactory::GetInstance()->GetParticle("Json/ParticleData_SmokeEmitter.json"));

		abilityDirection = { 0.0f, 0.0f, 0.0f };

		behavior = new CProjectileBehavior(abilityDirection, 3.0f);
		abilityTest->AddComponent<CAbilityBehaviorComponent>(*abilityTest, behavior, EAbilityType::TRIANGLE);

		abilityTest->AddComponent<CTriangleColliderComponent>(*abilityTest, 2.0f, 2.0f, ECollisionLayer::PLAYERABILITY, static_cast<int>(ECollisionLayer::ENEMY));

		abilityTest->Active(false);
		CScene::GetInstance()->AddInstance(abilityTest);
		break;
	case EAbilityType::BOX:
		abilityTest->myTransform->Position({ 0.0f, 0.0f, 0.0f });
		abilityTest->AddComponent<CVFXComponent>(*abilityTest);
		abilityTest->GetComponent<CVFXComponent>()->Init(CVFXFactory::GetInstance()->GetVFXBase("Assets/3D/VFX/VFX_mesh_disc_01_19G4.fbx", "Json/VFXData_FogWall.json"));

		abilityTest->AddComponent<CParticleEmitterComponent>(*abilityTest);
		abilityTest->GetComponent<CParticleEmitterComponent>()->Init(CParticleFactory::GetInstance()->GetParticle("Json/ParticleData_SmokeEmitter.json"));

		abilityDirection = { 0.0f, 0.0f, 0.0f };

		behavior = new CProjectileBehavior(abilityDirection, 3.0f);
		abilityTest->AddComponent<CAbilityBehaviorComponent>(*abilityTest, behavior, EAbilityType::BOX);

		abilityTest->AddComponent<CRectangleColliderComponent>(*abilityTest, 1.0f, 1.0f, ECollisionLayer::PLAYERABILITY, static_cast<int>(ECollisionLayer::ENEMY));

		abilityTest->Active(false);
		CScene::GetInstance()->AddInstance(abilityTest);
		break;
	default:
		break;
	}
	return abilityTest;
}
