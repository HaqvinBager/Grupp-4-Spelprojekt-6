#include "stdafx.h"
#include "Ability.h"

#include <fstream>
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"

//CAbility::SAbilityData& CAbility::GetAbilityData()
//{
//    // TODO: insert return statement here
//}

void CAbility::Init(std::string aFilePath)
{
	//Json reading
    using namespace rapidjson;

    std::ifstream inputStream(aFilePath);
    IStreamWrapper inputWrapper(inputStream);
    Document document;
    document.ParseStream(inputWrapper);

    //myData.myCollider = Collider(document["Collider"]["Type"].GetString(), document["Collider"],);
}

void CAbility::Update(float /*aDeltaTime*/)
{
	//Update behavior
}
