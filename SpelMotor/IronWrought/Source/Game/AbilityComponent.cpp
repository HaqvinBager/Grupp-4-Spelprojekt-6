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
#include "EngineException.h"

#include <fstream>
#include "rapidjson\document.h"
#include "rapidjson\istreamwrapper.h"

using namespace rapidjson;

CAbilityComponent::CAbilityComponent(CGameObject& aParent, std::vector<std::pair<EAbilityType, unsigned int>> someAbilities)
	: CBehaviour(aParent), myAbilityPoolDescriptions(someAbilities), myCurrentCooldowns(new float[3]), myMaxCooldowns(new float[3])
{
	myCurrentCooldowns[0] = 0.0f;
	myCurrentCooldowns[1] = 0.0f;
	myCurrentCooldowns[2] = 0.0f;

	myMaxCooldowns[0] = 0.2f; //TODO: make read unity
	myMaxCooldowns[1] = 1.0f; //TODO: make read unity
	myMaxCooldowns[2] = 5.0f; //TODO: make read unity

	std::ifstream inputStream("Json/AbilityPaths.json");
	ENGINE_BOOL_POPUP(inputStream.good(), "Ability json paths could not be found! Looking for Json/AbilityPaths.json");
	IStreamWrapper inputWrapper(inputStream);
	Document document;
	document.ParseStream(inputWrapper);

	myFilePaths.emplace(EAbilityType::PlayerAbility1, document["Ability Test"].GetString());
	myFilePaths.emplace(EAbilityType::PlayerAbility2, document["Player Ability 2"].GetString());
	myFilePaths.emplace(EAbilityType::PlayerAbility3, document["Ability Test"].GetString());
	myFilePaths.emplace(EAbilityType::PlayerAbility4, document["Ability Test"].GetString());
	myFilePaths.emplace(EAbilityType::PlayerAbility5, document["Ability Test"].GetString());
	myFilePaths.emplace(EAbilityType::EnemyAbility, document["Ability Test"].GetString());
	myFilePaths.emplace(EAbilityType::BossAbility1, document["Ability Test"].GetString());
	myFilePaths.emplace(EAbilityType::BossAbility2, document["Ability Test"].GetString());
	myFilePaths.emplace(EAbilityType::BossAbility3, document["Ability Test"].GetString());
	myFilePaths.emplace(EAbilityType::AbilityTest, document["Ability Test"].GetString());
	myFilePaths.emplace(EAbilityType::WHIRLWIND, document["Ability Test"].GetString());
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
	CInputMapper::GetInstance()->AddObserver(IInputObserver::EInputEvent::Ability1, this);
	CInputMapper::GetInstance()->AddObserver(IInputObserver::EInputEvent::Ability2, this);
	CInputMapper::GetInstance()->AddObserver(IInputObserver::EInputEvent::Ability3, this);

	// Setting up pools
	for (unsigned int i = 0; i < myAbilityPoolDescriptions.size(); ++i) {
		std::vector<CGameObject*> gameObjectsToPool;
		for (unsigned int j = 0; j < myAbilityPoolDescriptions[i].second; ++j) {
			gameObjectsToPool.emplace_back(LoadAbilityFromFile(myAbilityPoolDescriptions[i].first));
			CEngine::GetInstance()->GetActiveScene().AddInstance(gameObjectsToPool.back());
		}
		myAbilityPools.emplace(myAbilityPoolDescriptions[i].first, gameObjectsToPool);
	}
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

	if (myAbilityPools.find(anAbilityType) == myAbilityPools.end()) {
		return;
	}

	if (myAbilityPools.at(anAbilityType).empty()) {
		return;
	}

	myActiveAbilities.emplace_back(myAbilityPools.at(anAbilityType).back());
	myAbilityPools.at(anAbilityType).pop_back();
	myActiveAbilities.back()->Active(true);
	myActiveAbilities.back()->myTransform->Position(aSpawnPosition);

	// getparent().playanimation(myactiveabilities.back().getcomponent<pod>().myanimation);

	myActiveAbilities.back()->GetComponent<CAbilityBehaviorComponent>()->Init(aSpawnPosition);

	//switch (anAbilityType)
	//{
	//case EAbilityType::WHIRLWIND:
	//	break;
	//case EAbilityType::AbilityTest:
	//	myActiveAbilities.back()->GetComponent<CAbilityBehaviorComponent>()->Init(aSpawnPosition);
	//	break;
	//default:
	//	break;
	//}
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

void CAbilityComponent::ReceiveEvent(const EInputEvent aEvent)
{
	float messageValue = 1.0f;


	switch (aEvent)
	{
		SMessage myMessage;
	case EInputEvent::Ability1:
		if (myCurrentCooldowns[0] > 0)
			break;
		UseAbility(EAbilityType::PlayerAbility1, GameObject().myTransform->Position());
		myMessage.myMessageType = EMessageType::AbilityOneCooldown;
		myCurrentCooldowns[0] = myMaxCooldowns[0];
		myMessage.data = &messageValue;
		CMainSingleton::PostMaster().Send(myMessage);
		break;
	case EInputEvent::Ability2:
		if (myCurrentCooldowns[1] > 0)
			break;
		UseAbility(EAbilityType::PlayerAbility2, GameObject().myTransform->Position());
		myMessage.myMessageType = EMessageType::AbilityTwoCooldown;
		myCurrentCooldowns[1] = myMaxCooldowns[1];
		myMessage.data = &messageValue;
		CMainSingleton::PostMaster().Send(myMessage);
		break;
	case EInputEvent::Ability3:
		if (myCurrentCooldowns[2] > 0)
			break;
		UseAbility(EAbilityType::PlayerAbility3, GameObject().myTransform->Position());
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
	std::ifstream inputStream(myFilePaths[anAbilityType]);
	if (!inputStream.good()) { return nullptr; }
	IStreamWrapper inputWrapper(inputStream);
	Document document;
	document.ParseStream(inputWrapper);

	CGameObject* abilityObject = new CGameObject();
	CProjectileBehavior* projectileBehavior = nullptr;
	CAuraBehavior* auraBehavior = nullptr;
	std::string colliderType;

	//VFX
	abilityObject->myTransform->Position({ 0.0f, 0.0f, 0.0f });
	std::vector<std::string> paths;
	for (unsigned int i = 0; i < document["VFX"].Size(); ++i) {
		paths.emplace_back(document["VFX"][i]["Path"].GetString());
	}
	abilityObject->AddComponent<CVFXComponent>(*abilityObject);
	abilityObject->GetComponent<CVFXComponent>()->Init(CVFXFactory::GetInstance()->GetVFXBaseSet(paths));
	//!VFX

	//PARTICLESYSTEM
	paths.clear();
	for (unsigned int i = 0; i < document["ParticleSystems"].Size(); ++i) {
		paths.emplace_back(document["ParticleSystems"][i]["Path"].GetString());
	}
	abilityObject->AddComponent<CParticleEmitterComponent>(*abilityObject);
	abilityObject->GetComponent<CParticleEmitterComponent>()->Init(CParticleFactory::GetInstance()->GetParticleSet(paths));
	//!PARTICLESYSTEM

	//BEHAVIOR
	auto behavior = document["Behavior"].GetObjectW();
	if (behavior["Type"].GetString() == std::string("Aura"))
	{
		auraBehavior = new CAuraBehavior(&GameObject(), behavior["Rotational Speed"].GetFloat());
		abilityObject->AddComponent<CAbilityBehaviorComponent>(*abilityObject, auraBehavior, anAbilityType);
	}
	else if (behavior["Type"].GetString() == std::string("Projectile"))
	{
		projectileBehavior = new CProjectileBehavior(behavior["Speed"].GetFloat(), behavior["Duration"].GetFloat());
		abilityObject->AddComponent<CAbilityBehaviorComponent>(*abilityObject, projectileBehavior, anAbilityType);
	}
	//!BEHAVIOR

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
	//!COLLIDER

	abilityObject->Active(false);
	CEngine::GetInstance()->GetActiveScene().AddInstance(abilityObject);

	return abilityObject;
}
