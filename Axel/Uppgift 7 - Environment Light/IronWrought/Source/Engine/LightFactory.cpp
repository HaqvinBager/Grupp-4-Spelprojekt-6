#include "stdafx.h"
#include "LightFactory.h"
#include "EnvironmentLight.h"
#include "Engine.h"
#include <DDSTextureLoader.h>

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
