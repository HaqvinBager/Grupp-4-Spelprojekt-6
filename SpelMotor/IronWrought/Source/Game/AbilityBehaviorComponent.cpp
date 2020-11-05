#include "stdafx.h"
#include "AbilityBehaviorComponent.h"
#include "GameObject.h"
#include "AbilityBehavior.h"

CAbilityBehaviorComponent::CAbilityBehaviorComponent(CGameObject& aParent, IAbilityBehavior* aBehavior) : CComponent(aParent), myBehavior(aBehavior)
{
}

CAbilityBehaviorComponent::~CAbilityBehaviorComponent()
{
	delete myBehavior;
	myBehavior = nullptr;
}

void CAbilityBehaviorComponent::Awake()
{
}

void CAbilityBehaviorComponent::Start()
{
}

void CAbilityBehaviorComponent::Update()
{
	myBehavior->Update(&GetParent());
}
