#include "stdafx.h"
#include "ModelInstance.h"

using namespace DirectX::SimpleMath;

CModelInstance::CModelInstance() {
	myModel = nullptr;
	myScale = 1.0f;
}

CModelInstance::~CModelInstance() {
	myModel = nullptr;
}

void CModelInstance::Init(CModel* aModel) {
	myModel = aModel;
	myTransform = Matrix();
}

void CModelInstance::SetTransform(Vector3 aPosition,Vector3 aRotation)
{
	SetRotation(aRotation);
	SetPosition(aPosition);
}

void CModelInstance::SetPosition(Vector3 aPosition)
{
	myTransform.Translation(aPosition);
}

void CModelInstance::SetRotation(Vector3 aRotation)
{
	Vector3 tempTranslation = myTransform.Translation();

	Matrix tempRotation = Matrix::CreateFromYawPitchRoll(
		DirectX::XMConvertToRadians(aRotation.y),
		DirectX::XMConvertToRadians(aRotation.x),
		DirectX::XMConvertToRadians(aRotation.z)
	);
	myTransform = tempRotation;
	myTransform *= Matrix::CreateScale(myScale);
	myTransform.Translation(tempTranslation);
}

void CModelInstance::Move(Vector3 aMovement)
{
	myTransform.Translation(myTransform.Translation() + myTransform.Right() * aMovement.x);
	myTransform.Translation(myTransform.Translation() + myTransform.Up() * aMovement.y);
	myTransform.Translation(myTransform.Translation() - myTransform.Forward() * aMovement.z);
}

void CModelInstance::Rotate(Vector3 aRotation)
{
	Vector3 tempTranslation = myTransform.Translation();
	Matrix tempRotation = Matrix::CreateFromYawPitchRoll(aRotation.y, aRotation.x, aRotation.z);
	myTransform *= tempRotation;
	myTransform.Translation(tempTranslation);
}

void CModelInstance::SetScale(float aScale)
{
	myScale = aScale;
	myTransform *= Matrix::CreateScale(myScale);
}
