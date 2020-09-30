#include "stdafx.h"
#include "ModelInstance.h"

using namespace DirectX::SimpleMath;

#define ENGINE_SCALE 0.01f

CModelInstance::CModelInstance() {
	myModel = nullptr;
}

CModelInstance::~CModelInstance() {
}

void CModelInstance::Init(CModel* aModel) {
	myModel = aModel;
	myTransform = Matrix();
}

void CModelInstance::SetTransform(Vector3 aPosition,Vector3 aRotation, Vector3 aScale)
{
	//SetScale(aScale);
	myScale = aScale;
	SetRotation(aRotation);
	SetPosition(aPosition);
}

void CModelInstance::SetPosition(Vector3 aPosition)
{
	myTransform.Translation(aPosition);
}

void CModelInstance::SetRotation(Vector3 aRotation)
{
	Vector3 translation = myTransform.Translation();
	myTransform = Matrix::CreateFromYawPitchRoll(aRotation.y, aRotation.x, aRotation.z);
	myTransform *= Matrix::CreateScale(myScale);
	//myTransform += Matrix::CreateTranslation(aRotation);
	myTransform.Translation(translation);
}

void CModelInstance::Move(Vector3 aMovement)
{
	myTransform._41 += aMovement.x;
	myTransform._42 += aMovement.y;
	myTransform._43 += aMovement.z;
}

void CModelInstance::Rotate(Vector3 aRotation)
{
	Vector3 tempTranslation = myTransform.Translation();
	myTransform.Translation({ 0,0,0 });
	myTransform *= Matrix::CreateFromYawPitchRoll(aRotation.y, aRotation.x, aRotation.z);
	myTransform.Translation(tempTranslation);
}

void CModelInstance::SetScale(DirectX::SimpleMath::Vector3 aScale)
{
	myScale = aScale;
	myTransform *= Matrix::CreateScale(myScale);

	//myTransform = myTransform.CreateScale(aScale);
}
