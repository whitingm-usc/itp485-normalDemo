#include "stdafx.h"
#include "SimpleRotate.h"
#include "game.h"
#include "jsonUtil.h"
#include "RenderObj.h"

SimpleRotate::SimpleRotate(RenderObj* pObj)
    : Component(pObj)
    , m_speed(0.0f)
{
}

void SimpleRotate::LoadProperties(const rapidjson::Value& properties)
{
    Component::LoadProperties(properties);
    GetFloatFromJSON(properties, "speed", m_speed);
}

void SimpleRotate::Update(float deltaTime)
{
    Matrix4 rot = Matrix4::CreateRotationY(-0.25f * m_speed * deltaTime)
		* Matrix4::CreateRotationZ(m_speed * deltaTime);
    mObj->mObjectData.c_modelToWorld = rot * mObj->mObjectData.c_modelToWorld;
}
