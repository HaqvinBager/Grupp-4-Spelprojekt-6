#include "stdafx.h"
#include "ModelInstance.h"

using namespace DirectX::SimpleMath;

CModelInstance::CModelInstance() {
	myModel = nullptr;
}

CModelInstance::~CModelInstance() {
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
	Vector3 translation = myTransform.Translation();
	myTransform = Matrix::CreateFromYawPitchRoll(aRotation.y, aRotation.x, aRotation.z);
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