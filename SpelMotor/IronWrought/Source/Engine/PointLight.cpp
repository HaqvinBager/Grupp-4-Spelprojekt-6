#include "stdafx.h"
#include "PointLight.h"
#include <SimpleMath.h>

CPointLight::CPointLight() : myPosition(), myColor(), myRange(), myIntensity()
{
}

CPointLight::~CPointLight()
{
}

bool CPointLight::Init()
{

	return true;
}