#pragma once
#include "SimpleMath.h"

class CCamera {
public:
	CCamera();
	~CCamera();

	bool Init(float aFoV, DirectX::SimpleMath::Vector2 aResolution);
	void SetTransform(DirectX::SimpleMath::Vector3 aPosition, DirectX::SimpleMath::Vector3 aRotation);
	void SetPosition(DirectX::SimpleMath::Vector3 aRotation);
	void SetRotation(DirectX::SimpleMath::Vector3 aPosition);
	void Move(DirectX::SimpleMath::Vector3 aMovement);
	void Rotate(DirectX::SimpleMath::Vector3 aRotation);

public:
	DirectX::SimpleMath::Matrix GetTransform() { return myTransform; }
	DirectX::SimpleMath::Matrix GetProjection() { return myProjection; }

private:
	DirectX::SimpleMath::Matrix myTransform;
	DirectX::SimpleMath::Matrix myProjection;

};