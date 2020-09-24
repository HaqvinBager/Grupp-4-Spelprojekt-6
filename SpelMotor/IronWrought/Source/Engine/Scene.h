#pragma once
#include <vector>

class CModelInstance;
class CCamera;
class CEnvironmentLight;
class CPointLight;

class CScene {

	friend class CEngine;

public:

	static CScene* GetInstance();
	bool Init();
	void SetMainCamera(CCamera* aCamera);
	CCamera* GetMainCamera();
	CEnvironmentLight* GetEnvironmentLight();
	std::vector<CModelInstance*> CullModels(CCamera* aMainCamera);
	const std::vector<CPointLight*>& GetPointLights();

	std::pair<unsigned int, std::array<CPointLight*, 8>> CullLights(CModelInstance* aModel);

	bool AddInstance(CModelInstance* aModel);
	bool AddInstance(CCamera* aCamera);
	bool AddInstance(CEnvironmentLight* anEnvironmentLight);
	bool AddInstance(CPointLight* aPointLight);
private:
	CScene();
	~CScene();

private:
	CCamera* myMainCamera;
	std::vector<CCamera*> myCameras;
	std::vector<CModelInstance*> myModelInstances;
	std::vector<CEnvironmentLight*> myEnvironmentLights;
	std::vector<CPointLight*> myPointLights;

	static CScene* ourInstance;
};