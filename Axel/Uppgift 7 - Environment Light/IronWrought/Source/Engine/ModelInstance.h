#pragma once
#include "SimpleMath.h"

class CModel;

class CModelInstance {
public:
	CModelInstance();
	~CModelInstance();	

	void Init(CModel* aModel);
	void SetTransform(DirectX::SimpleMath::Vector3 aPosition, DirectX::SimpleMath::Vector3 aRotation);
	void SetPosition(DirectX::SimpleMath::Vector3 aPosition);
	void SetRotation(DirectX::SimpleMath::Vector3 aRotation);
	void Move(DirectX::SimpleMath::Vector3 aMovement);
	void Rotate(DirectX::SimpleMath::Vector3 aRotation);


public:
	CModel* GetModel() { return myModel; }
	DirectX::SimpleMath::Matrix GetTransform() { return myTransform; };
	DirectX::SimpleMath::Vector2 GetUVOffset() { return myUVOffset; };
	void SetUVOffset(DirectX::SimpleMath::Vector2 aUVOffset) { myUVOffset = aUVOffset; };

private:
	CModel* myModel;
	DirectX::SimpleMath::Matrix myTransform;
	DirectX::SimpleMath::Vector2 myUVOffset;
};