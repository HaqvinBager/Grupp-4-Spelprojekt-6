#pragma once
#include "StateStack.h"

class CGame
{
public:
	CGame();
	~CGame();

	void Init();
	void Update();

	unsigned int myWindowWidth;
	unsigned int myWindowHeight;

private:
	CStateStack myStateStack;
};

