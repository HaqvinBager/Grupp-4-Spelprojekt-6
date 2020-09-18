#include "stdafx.h"
#include "Camera.h"

using namespace DirectX::SimpleMath;

CCamera::CCamera()
{
}

CCamera::~CCamera()
{
}

bool CCamera::Init(float aFoV, Vector2 aResolution)
{
	myTransform = Matrix();
	myProjection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(aFoV), (aResolution.x / aResolution.y), 0.05f, 100.0f);
	return true;
}

void CCamera::SetTransform(Vector3 aPosition, Vector3 aRotation)
{
	SetRotation(aRotation);
	SetPosition(aPosition);
}

void CCamera::SetPosition(Vector3 aPosition)
{
	myTransform.Translation(aPosition * -1.0f);
}

void CCamera::SetRotation(Vector3 aRotation)
{
	myTransform = Matrix::CreateFromYawPitchRoll(DirectX::XMConvertToRadians(aRotation.y), 
												 DirectX::XMConvertToRadians(aRotation.x), 
												 DirectX::XMConvertToRadians(aRotation.z));
}

void CCamera::Move(Vector3 aMovement)
{
	myTransform._41 -= aMovement.x;
	myTransform._42 -= aMovement.y;
	myTransform._43 -= aMovement.z;
}

void CCamera::Rotate(Vector3 aRotation)
{
	myTransform *= Matrix::CreateFromYawPitchRoll(aRotation.y, aRotation.x, aRotation.z);
}