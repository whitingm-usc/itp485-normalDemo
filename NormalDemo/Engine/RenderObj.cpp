#include "stdafx.h"
#include "RenderObj.h"
#include "Components\Component.h"
#include "Graphics.h"
#include "mesh.h"


RenderObj::RenderObj(Graphics* graphics, const Mesh* pMesh)
    : mGraphics(graphics)
    , mMesh(pMesh)
{
    mObjectData.c_modelToWorld = Matrix4::CreateRotationZ(Math::ToRadians(45.0f));
    mObjectBuffer = mGraphics->CreateGraphicsBuffer(&mObjectData, sizeof(mObjectData), D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC); 
}

/*virtual*/ RenderObj::~RenderObj()
{
    mObjectBuffer->Release();
}

/*virtual*/ void RenderObj::Draw()
{
    mGraphics->UploadBuffer(mObjectBuffer, &mObjectData, sizeof(mObjectData));
    mGraphics->GetDeviceContext()->VSSetConstantBuffers(Graphics::CONSTANT_BUFFER_RENDEROBJ, 1, &mObjectBuffer);
    mMesh->Draw();
}

/*virtual*/ void RenderObj::Update(float deltaTime)
{
    for (Component* pComp : mComponents)
    {
        pComp->Update(deltaTime);
    }
}
