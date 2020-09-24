#include "stdafx.h"
#include "LightFactory.h"
#include <DDSTextureLoader.h>
#include "Engine.h"
#include "EnvironmentLight.h"
#include "PointLight.h"

CLightFactory* CLightFactory::ourInstance = nullptr;
CLightFactory* CLightFactory::GetInstance()
{
    return ourInstance;
}
bool CLightFactory::Init(CEngine& anEngine)
{
    myEngine = &anEngine;
    return true;
}
CEnvironmentLight* CLightFactory::CreateEnvironmentLight(std::string aCubeMapPath)
{
	CEnvironmentLight* light = new CEnvironmentLight();
    if (!light->Init(myEngine->myFramework, aCubeMapPath))
    {
        return nullptr;
    }
    
    return light;
}
CPointLight* CLightFactory::CreatePointLight()
{
    CPointLight* light = new CPointLight();
    if (!light->Init())
    {
        return nullptr;
    }

    return light;
}
CLightFactory::CLightFactory()
{
    ourInstance = this;
    myEngine = nullptr;
}

CLightFactory::~CLightFactory()
{
    ourInstance = nullptr;
    myEngine = nullptr;
}
