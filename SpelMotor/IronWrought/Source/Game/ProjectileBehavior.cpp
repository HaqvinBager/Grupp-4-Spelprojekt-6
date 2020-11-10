#include "stdafx.h"
#include "ProjectileBehavior.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Timer.h"
#include "MouseTracker.h"
#include "Engine.h"

namespace SM = DirectX::SimpleMath;

CProjectileBehavior::CProjectileBehavior(SM::Vector3 aDirection, float aSpeed)
{
	myDirection = aDirection;
	myDirection.Normalize();
	mySpeed = aSpeed;

	myDuration = 3.0f;
	myTimer = 0.0f;
}

CProjectileBehavior::~CProjectileBehavior()
{
}

void CProjectileBehavior::Update(CGameObject* aParent)
{
	myTimer += CTimer::Dt();
	if (myTimer > myDuration) {
		myTimer = 0.0f;
		//Backup
		//aParent->Enabled(false);
		aParent->SetActive(false);
	}
	aParent->GetComponent<CTransformComponent>()->Move(myDirection * mySpeed * CTimer::Dt());
}

void CProjectileBehavior::Init(DirectX::SimpleMath::Vector3 aCasterPosition)
{
	myDirection = MouseTracker::ScreenPositionToWorldPosition(CEngine::GetInstance()->GetWindowHandler()->GetWidth(), CEngine::GetInstance()->GetWindowHandler()->GetHeight()) - aCasterPosition;
	myDirection.Normalize();
}
