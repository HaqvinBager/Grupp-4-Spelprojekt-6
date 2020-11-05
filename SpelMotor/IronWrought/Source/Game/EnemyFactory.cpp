#include "stdafx.h"
#include "EnemyFactory.h"

CEnemyFactory* CEnemyFactory::GetInstance()
{
	return ourInstance;
}

CEnemyFactory::CEnemyFactory() {
	ourInstance = this;
}

CEnemyFactory::~CEnemyFactory()
{
	ourInstance = nullptr;
}
