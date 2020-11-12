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

using namespace CommonUtilities;

CInGameState::CInGameState(CStateStack& aStateStack) : CState(aStateStack) {
	CSpriteInstance* spriteInstance = new CSpriteInstance();
	spriteInstance->Init(CSpriteFactory::GetInstance()->GetSprite("tempUI.dds"));
	spriteInstance->SetSize({ 2.0f,2.0f });
	spriteInstance->SetPosition({ 0.0f,-0.85f });
	CScene::GetInstance()->AddInstance(spriteInstance);

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
