#include "stdafx.h"
#include "ProjectileBehavior.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Timer.h"

namespace SM = DirectX::SimpleMath;

CProjectileBehavior::CProjectileBehavior(SM::Vector3 aDirection, float aSpeed)
{
	myDirection = aDirection;
	myDirection.Normalize();
	mySpeed = aSpeed;

	myDuration = 1.0f;
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
		aParent->Enabled(false);
	}
	aParent->GetComponent<CTransformComponent>()->Move(myDirection * mySpeed * CTimer::Dt());
}
