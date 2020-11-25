#pragma once
#include "StateStack.h"

class CGame
{
public:
	CGame();
	~CGame();

	void Init();
	void Update();

private:
	CStateStack myStateStack;
};

