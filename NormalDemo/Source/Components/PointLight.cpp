#include "stdafx.h"
#include "PointLight.h"
#include "jsonUtil.h"
#include "RenderObj.h"

PointLight::PointLight(Game* pGame, RenderObj* pObj)
    : Component(pObj)
    , mGame(pGame)
{
    mLight = mGame->AllocateLight();
}

PointLight::~PointLight()
{
    mGame->FreeLight(mLight);
}

void PointLight::LoadProperties(const rapidjson::Value& properties)
{
    GetVectorFromJSON(properties, "diffuseColor", mLight->diffuseColor);
    GetVectorFromJSON(properties, "specularColor", mLight->specularColor);
    GetFloatFromJSON(properties, "specularPower", mLight->specularPower);
    GetFloatFromJSON(properties, "innerRadius", mLight->innerRadius);
    GetFloatFromJSON(properties, "outerRadius", mLight->outerRadius);
}

void PointLight::Update(float deltaTime)
{
    mLight->position = mObj->mObjectData.c_modelToWorld.GetTranslation();
}
