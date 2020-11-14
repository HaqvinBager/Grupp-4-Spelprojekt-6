#include "stdafx.h"
#include "InGameState.h"
#include "StateStack.h"
#include "Scene.h"
#include "GameObject.h"
#include "SpriteInstance.h"
#include "SpriteFactory.h"
#include "AnimationComponent.h"
#include "ModelComponent.h"
#include "Model.h"
#include "Animation.h"
#include "TransformComponent.h"
#include "DialogueSystem.h"
#include "Timer.h"
#include "CameraComponent.h"
#include "Engine.h"
#include "WindowHandler.h"
#include "AnimatedUIElement.h"

//collider test
#include "CircleColliderComponent.h"
#include "RectangleColliderComponent.h"
#include "TriangleColliderComponent.h"
//collider test


using namespace CommonUtilities;

CInGameState::CInGameState(CStateStack& aStateStack) : CState(aStateStack) {
	CSpriteInstance* spriteInstance = new CSpriteInstance();
	spriteInstance->Init(CSpriteFactory::GetInstance()->GetSprite("Assets/3D/UI/Ingame/UI_test.dds"/*"tempUI.dds"*/));
	spriteInstance->SetSize({ 1024.0f / 1000.0f, 256.0f / 1000.0f });
	//spriteInstance->SetSize({ 1024.0f / CEngine::GetInstance()->GetWindowHandler()->GetWidth(), 256.0f / CEngine::GetInstance()->GetWindowHandler()->GetWidth() });
	//spriteInstance->SetSize({ 2.0f, 2.0f });
	//spriteInstance->SetPosition({ 0.0f,-0.85f });
	spriteInstance->SetPosition({ 0.0f,-0.82f });
	CScene::GetInstance()->AddInstance(spriteInstance);

	CAnimatedUIElement* uiElement = new CAnimatedUIElement();
	CScene::GetInstance()->AddInstance(uiElement);

	//CGameObject* enemy = new CGameObject();
	//enemy->AddComponent<CTransformComponent>(*enemy);
	//enemy->AddComponent<CModelComponent>(*enemy, "Assets/3D/Character/Enemy1/enemy_sk.fbx");

	//CAnimation* anim = new CAnimation();
	//std::vector < std::string> animationClips = {
	//	"Assets/3D/Character/Enemy1/enemy_excitedJump.fbx"
	//};
	//anim->Init("Assets/3D/Character/Enemy1/enemy_sk.fbx", animationClips);
	//enemy->AddComponent<CAnimationComponent>(*enemy);
	//CScene::GetInstance()->AddInstance(enemy);

	CGameObject* colliderBoi0 = new CGameObject();
	DirectX::SimpleMath::Vector3 pos = { 0.0f, 0.0f, 0.0f };
	colliderBoi0->AddComponent<CTransformComponent>(*colliderBoi0, pos);
	colliderBoi0->GetComponent< CTransformComponent>()->Position(pos);
	colliderBoi0->AddComponent<CRectangleColliderComponent>(*colliderBoi0, 2.0f, 2.0f, ECollisionLayer::ENEMY, static_cast<int>(ECollisionLayer::PLAYER) | static_cast<int>(ECollisionLayer::PLAYERABILITY));
	colliderBoi0->AddComponent<CModelComponent>(*colliderBoi0, "Assets/3D/Character/CH_NPC_enemy_01_19G4_1_19/CH_NPC_enemy_01_19G4_1_19.fbx");

	CGameObject* colliderBoi50 = new CGameObject();
	DirectX::SimpleMath::Vector3 pos50 = { 0.0f, 0.0f, 0.0f };
	colliderBoi50->AddComponent<CTransformComponent>(*colliderBoi50, pos50);
	colliderBoi50->GetComponent< CTransformComponent>()->Position(pos50);
	colliderBoi50->AddComponent<CModelComponent>(*colliderBoi50, "Assets/3D/Character/CH_NPC_enemy_01_19G4_1_19/CH_NPC_enemy_01_19G4_1_19.fbx");
	colliderBoi50->AddComponent<CCircleColliderComponent>(*colliderBoi50, 1.0f, ECollisionLayer::PLAYER, static_cast<int>(ECollisionLayer::ENEMY) | static_cast<int>(ECollisionLayer::BOSS));

	CGameObject* colliderBoi500 = new CGameObject();
	DirectX::SimpleMath::Vector3 pos500 = { 0.0f, 0.0f, 0.0f };
	colliderBoi500->AddComponent<CTransformComponent>(*colliderBoi500, pos500);
	colliderBoi500->GetComponent< CTransformComponent>()->Position(pos500);
	colliderBoi500->AddComponent<CModelComponent>(*colliderBoi500, "Assets/3D/Character/CH_NPC_enemy_01_19G4_1_19/CH_NPC_enemy_01_19G4_1_19.fbx");
	colliderBoi500->AddComponent<CTriangleColliderComponent>(*colliderBoi500, 2.0f, 2.0f, ECollisionLayer::ENEMYABILITY, static_cast<int>(ECollisionLayer::PLAYERABILITY));

	CScene::GetInstance()->AddInstance(colliderBoi0);
	CScene::GetInstance()->AddInstance(colliderBoi50);
	CScene::GetInstance()->AddInstance(colliderBoi500);

	myDialogueSystem = new CDialogueSystem();
}

CInGameState::~CInGameState()
{

}

void CInGameState::Awake()
{
	for (auto& gameObject : CScene::GetInstance()->myGameObjects)
	{
		gameObject->Awake();
	}
}

void CInGameState::Start()
{
	for (auto& gameObject : CScene::GetInstance()->myGameObjects)
	{
		gameObject->Start();
	}
}

void CInGameState::Update()
{
	for (auto& gameObject : CScene::GetInstance()->myGameObjects)
	{
		gameObject->Update();
	}

	std::vector<CSpriteInstance*> animatedUIFrames;
	auto animUI = CScene::GetInstance()->CullAnimatedUI(animatedUIFrames)[0];
	animUI->Level(abs(sin(CTimer::Time())));

	CCollisionManager::GetInstance()->Update();

	myDialogueSystem->Update(CTimer::Dt());

	CCameraComponent* camera = CScene::GetInstance()->GetMainCamera();

	const float dt = CTimer::Dt();
	float cameraMoveSpeed = 10.0f;
	float verticalMoveSpeedModifier = 0.5f;
	DirectX::SimpleMath::Vector3 camera_movement_input(0, 0, 0);
	camera_movement_input.z = Input::GetInstance()->IsKeyDown('W') ? cameraMoveSpeed : camera_movement_input.z;
	camera_movement_input.z = Input::GetInstance()->IsKeyDown('S') ? -cameraMoveSpeed : camera_movement_input.z;
	camera_movement_input.x = Input::GetInstance()->IsKeyDown('D') ? cameraMoveSpeed : camera_movement_input.x;
	camera_movement_input.x = Input::GetInstance()->IsKeyDown('A') ? -cameraMoveSpeed : camera_movement_input.x;
	camera_movement_input.y = Input::GetInstance()->IsKeyDown('R') ? cameraMoveSpeed * verticalMoveSpeedModifier : camera_movement_input.y;
	camera_movement_input.y = Input::GetInstance()->IsKeyDown('F') ? -cameraMoveSpeed * verticalMoveSpeedModifier : camera_movement_input.y;

	float rotationSpeed = 1.0f;
	float camera_rotation_input = 0;
	camera_rotation_input = Input::GetInstance()->IsKeyDown('Q') ? -rotationSpeed : camera_rotation_input;
	camera_rotation_input = Input::GetInstance()->IsKeyDown('E') ? rotationSpeed : camera_rotation_input;

	camera->GameObject().myTransform->MoveLocal(camera_movement_input * cameraMoveSpeed * dt);
	camera->GameObject().myTransform->Rotate({ 0, camera_rotation_input * rotationSpeed * dt, 0 });
}
