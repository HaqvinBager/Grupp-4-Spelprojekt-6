#pragma once
#include "State.h"

class CTokenPool;
class CCanvas;
class CGameObject;
class CInGameState : public CState
{

public:
	CInGameState(CStateStack& aStateStack);
	~CInGameState() override;

	void Awake() override;
	void Start() override;
	void Update() override;

	void MakeSceneActive() override;

private:
	CCanvas* myCanvas;
	CTokenPool* myTokenPool;
	CGameObject* myEnemy;
};