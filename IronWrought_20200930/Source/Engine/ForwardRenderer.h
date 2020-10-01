#pragma once
#include <vector>
#include "SimpleMath.h"

class CModelInstance;
class CEngine;
class CCamera;
class CEnvironmentLight;

class CForwardRenderer {
public:
	CForwardRenderer();
	~CForwardRenderer();

	bool Init(CEngine& anEngine);
	void Render(CEnvironmentLight* anEnvironmentLight, CCamera* aCamera, std::vector<CModelInstance*>& aModelList);

private:
	struct SFrameBufferData {
		DirectX::SimpleMath::Matrix myToCamera;
		DirectX::SimpleMath::Matrix myToProjection;
		DirectX::SimpleMath::Vector4 myDirectionalLightDirection;
		DirectX::SimpleMath::Vector4 myDirectionalLightColor;
	} myFrameBufferData;

	struct SObjectBufferData {
		DirectX::SimpleMath::Matrix myToWorld;

	} myObjectBufferData;

private:

	ID3D11DeviceContext* myContext;
	ID3D11Buffer* myFrameBuffer;
	ID3D11Buffer* myObjectBuffer;
};