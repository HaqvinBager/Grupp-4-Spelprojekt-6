#pragma once
#include "State.h"

class CDialogueSystem;


class CInGameState : public CState
{

public:
	CInGameState(CStateStack& aStateStack);
	~CInGameState() override;

	void Awake() override;
	void Start() override;
	void Update() override;
private:
	CDialogueSystem* myDialogueSystem;
};