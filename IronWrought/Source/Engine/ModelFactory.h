#pragma once
#include <string>
#include <map>

class CModel;
class CEngine;

struct ID3D11Device;

class CModelFactory {
	friend class CEngine;
public:
	static CModelFactory* GetInstance();
	bool Init(CEngine& engine);
	CModel* GetModel(std::string key);
	CModel* LoadModel(std::string aFilePath);
	CModel* GetCube();

private:
	CModelFactory();
	~CModelFactory();

	std::map<std::string, CModel*> myModelMap;
	CEngine* myEngine;
	static CModelFactory* ourInstance;
};