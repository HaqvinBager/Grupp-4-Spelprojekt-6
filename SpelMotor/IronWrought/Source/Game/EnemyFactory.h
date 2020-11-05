#pragma once
class CEnemyFactory
{
public:
	static CEnemyFactory* GetInstance();

private:
	CEnemyFactory();
	~CEnemyFactory();
private:
	static CEnemyFactory* ourInstance;
};

