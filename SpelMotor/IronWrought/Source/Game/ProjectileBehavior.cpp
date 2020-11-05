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
}

CProjectileBehavior::~CProjectileBehavior()
{
}

void CProjectileBehavior::Update(CGameObject* aParent)
{
	aParent->GetComponent<CTransformComponent>()->Move(myDirection * mySpeed * CTimer::Dt());
	//someAbilityData.myCollider->SetPosition(aParent.Position() + direction);
	//someAbilityData.myVFX->SetPosition(aParent.Position() + direction);
	//for (unsigned int i = 0; i < someAbilityData.myParticleSystems.size(); ++i) {
	//	someAbilityData.myParticleSystems[i]->SetPosition(aParent.Position() + direction);
	//}
}
