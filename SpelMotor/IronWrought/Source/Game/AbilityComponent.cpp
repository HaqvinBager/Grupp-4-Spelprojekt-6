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
#include "AuraBehavior.h"
#include "TransformComponent.h"
#include "Scene.h"
#include "InputMapper.h"
#include "MainSingleton.h"
#include "Engine.h"

#include <fstream>
#include "rapidjson\document.h"
#include "rapidjson\istreamwrapper.h"

CAbilityComponent::CAbilityComponent(CGameObject& aParent, std::vector<std::pair<EAbilityType, unsigned int>> someAbilities) : CBehaviour(aParent), myCurrentCooldowns(new float[3]), myMaxCooldowns(new float[3])
{
	CInputMapper::GetInstance()->AddObserver(IInputObserver::EInputEvent::Ability1, this);
	CInputMapper::GetInstance()->AddObserver(IInputObserver::EInputEvent::Ability2, this);
	CInputMapper::GetInstance()->AddObserver(IInputObserver::EInputEvent::Ability3, this);

	myCurrentCooldowns[0] = 0.0f;
	myCurrentCooldowns[1] = 0.0f;
	myCurrentCooldowns[2] = 0.0f;

	myMaxCooldowns[0] = 0.2f; //TODO: make read unity
	myMaxCooldowns[1] = 50.0f * 0.9f; //TODO: make read unity
	myMaxCooldowns[2] = 60.f*60.f; //TODO: make read unity

	myFilePaths.emplace(EAbilityType::WHIRLWIND, "Json/AbilityTest.json");
	myFilePaths.emplace(EAbilityType::AbilityTest, "Json/AbilityTest.json");

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
	delete myCurrentCooldowns;
	myCurrentCooldowns = nullptr;

	delete myMaxCooldowns;
	myMaxCooldowns = nullptr;
}

void CAbilityComponent::Awake()
{
}

void CAbilityComponent::Start()
{
}

void CAbilityComponent::Update()
{
	SendEvent();

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

	switch (anAbilityType)
	{
	case EAbilityType::WHIRLWIND:
		myActiveAbilities.back()->GetComponent<CAbilityBehaviorComponent>()->Init(aSpawnPosition);
		break;
	case EAbilityType::TRIANGLE:
		myActiveAbilities.back()->GetComponent<CAbilityBehaviorComponent>()->Init(aSpawnPosition);
		break;
	case EAbilityType::BOX:
		myActiveAbilities.back()->GetComponent<CAbilityBehaviorComponent>()->Init(aSpawnPosition);
		break;
	case EAbilityType::AbilityTest:
		myActiveAbilities.back()->GetComponent<CAbilityBehaviorComponent>()->Init(aSpawnPosition);
		break;
	default:
		break;
	}
}

void CAbilityComponent::SendEvent() {
	//ebin codez
	if (myCurrentCooldowns[0] > 0) {
		SMessage myMessage;
		myCurrentCooldowns[0] -= CTimer::Dt();
		float messageValue = myCurrentCooldowns[0] / myMaxCooldowns[0];
		myMessage.myMessageType = EMessageType::AbilityOneCooldown;
		myMessage.data = &messageValue;
		CMainSingleton::PostMaster().Send(myMessage);
	}

	if (myCurrentCooldowns[1] > 0) {
		SMessage myMessage;
		myCurrentCooldowns[1] -= CTimer::Dt();

		float messageValue = myCurrentCooldowns[1] / myMaxCooldowns[1];
		myMessage.myMessageType = EMessageType::AbilityTwoCooldown;

		myMessage.data = &messageValue;
		CMainSingleton::PostMaster().Send(myMessage);

	}
	if (myCurrentCooldowns[2] > 0) {
		SMessage myMessage;
		myCurrentCooldowns[2] -= CTimer::Dt();

		float messageValue = myCurrentCooldowns[2] / myMaxCooldowns[2];
		myMessage.myMessageType = EMessageType::AbilityThreeCooldown;

		myMessage.data = &messageValue;
		CMainSingleton::PostMaster().Send(myMessage);
	}
}

void CAbilityComponent::RecieveEvent(const EInputEvent aEvent)
{
	float messageValue = 1.0f;

	
	switch (aEvent)
	{
		SMessage myMessage;
	case EInputEvent::Ability1:
		if (myCurrentCooldowns[0] > 0)
			break;
		myMessage.myMessageType = EMessageType::AbilityOneCooldown;
		myCurrentCooldowns[0] = myMaxCooldowns[0];
		myMessage.data = &messageValue;
		CMainSingleton::PostMaster().Send(myMessage);
		break;
	case EInputEvent::Ability2:
		if (myCurrentCooldowns[1] > 0)
			break;
		myMessage.myMessageType = EMessageType::AbilityTwoCooldown;
		myCurrentCooldowns[1] = myMaxCooldowns[1];
		myMessage.data = &messageValue;
		CMainSingleton::PostMaster().Send(myMessage);
		break;
	case EInputEvent::Ability3:
		if (myCurrentCooldowns[2] > 0)
			break;
		myMessage.myMessageType = EMessageType::AbilityThreeCooldown;
		myCurrentCooldowns[2] = myMaxCooldowns[2];
		myMessage.data = &messageValue;
		CMainSingleton::PostMaster().Send(myMessage);
		break;
	default:
		break;
	}
}

CGameObject* CAbilityComponent::LoadAbilityFromFile(EAbilityType anAbilityType)
{
	using namespace rapidjson;

	std::ifstream inputStream(myFilePaths[anAbilityType]);
	if (!inputStream.good()) { return nullptr; }
	IStreamWrapper inputWrapper(inputStream);
	Document document;
	document.ParseStream(inputWrapper);

	CGameObject* abilityObject = new CGameObject();
	CProjectileBehavior* projectileBehavior = nullptr;
	CAuraBehavior* auraBehavior = nullptr;
	DirectX::SimpleMath::Vector3 abilityDirection;
	std::string colliderType;
	
	//VFX
	abilityObject->myTransform->Position({ 0.0f, 0.0f, 0.0f });
	for (unsigned int i = 0; i < document["VFX"].Size(); ++i) {
		abilityObject->AddComponent<CVFXComponent>(*abilityObject);
		abilityObject->GetComponent<CVFXComponent>()->Init(CVFXFactory::GetInstance()->GetVFXBase(document["VFX"][i]["Path"].GetString()));
	}
	//!VFX

	//PARTICLESYSTEM
	for (unsigned int i = 0; i < document["ParticleSystems"].Size(); ++i) {
		abilityObject->AddComponent<CParticleEmitterComponent>(*abilityObject);
		abilityObject->GetComponent<CParticleEmitterComponent>()->Init(CParticleFactory::GetInstance()->GetParticle(document["ParticleSystems"][i]["Path"].GetString()));
	}
	//!PARTICLESYSTEM
	
	//BEHAVIOR
	switch (anAbilityType)
	{
	case EAbilityType::WHIRLWIND:

		abilityDirection = { 0.0f, 0.0f, 0.0f };
		projectileBehavior = new CProjectileBehavior(abilityDirection, 3.0f);
		abilityObject->AddComponent<CAbilityBehaviorComponent>(*abilityObject, projectileBehavior, EAbilityType::WHIRLWIND);

		break;
	case EAbilityType::TRIANGLE:

		abilityDirection = { 0.0f, 0.0f, 0.0f };
		projectileBehavior = new CProjectileBehavior(abilityDirection, 3.0f);
		abilityObject->AddComponent<CAbilityBehaviorComponent>(*abilityObject, projectileBehavior, EAbilityType::TRIANGLE);

		break;
	case EAbilityType::BOX:

		abilityDirection = { 0.0f, 0.0f, 0.0f };
		projectileBehavior = new CProjectileBehavior(abilityDirection, 3.0f);
		abilityObject->AddComponent<CAbilityBehaviorComponent>(*abilityObject, projectileBehavior, EAbilityType::BOX);

		break;
	case EAbilityType::AbilityTest:

		auraBehavior = new CAuraBehavior(&GameObject(), 2.0f);
		abilityObject->AddComponent<CAbilityBehaviorComponent>(*abilityObject, auraBehavior, EAbilityType::AbilityTest);

		break;
	default:
		break;
	}
	//BEHAVIOR

	//COLLIDER
	colliderType = std::string(document["Collider"]["Type"].GetString());
	if (colliderType == "Circle") {
		abilityObject->AddComponent<CCircleColliderComponent>
			(
				*abilityObject,
				document["Collider"]["Width"].GetFloat(),
				static_cast<ECollisionLayer>(document["Collider"]["Collision Layer"].GetInt()),
				document["Collider"]["Collides With"].GetInt()
				);
	}
	else if (colliderType == "Rectangle") {
		abilityObject->AddComponent<CRectangleColliderComponent>
			(
				*abilityObject,
				document["Collider"]["Width"].GetFloat(),
				document["Collider"]["Height"].GetFloat(),
				static_cast<ECollisionLayer>(document["Collider"]["Collision Layer"].GetInt()),
				document["Collider"]["Collides With"].GetInt()
				);
	}
	else if (colliderType == "Triangle") {
		abilityObject->AddComponent<CTriangleColliderComponent>
			(
				*abilityObject,
				document["Collider"]["Width"].GetFloat(),
				document["Collider"]["Height"].GetFloat(),
				static_cast<ECollisionLayer>(document["Collider"]["Collision Layer"].GetInt()),
				document["Collider"]["Collides With"].GetInt()
				);
	}
	//COLLIDER

	abilityObject->Active(false);
	CEngine::GetInstance()->GetActiveScene().AddInstance(abilityObject);

	return abilityObject;
}
