#include "stdafx.h"
#include "SkinnedObj.h"
#include "Graphics.h"


SkinnedObj::SkinnedObj(Graphics* graphics, const Mesh* pMesh)
    : RenderObj(graphics, pMesh)
{
    for (int i = 0; i < MAX_SKELETON_BONES; ++i)
    {
        mSkinConstants.c_skinMatrix[i] = Matrix4::Identity;
    }
    mSkinBuffer = mGraphics->CreateGraphicsBuffer(&mSkinConstants, sizeof(mSkinConstants), D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
}

SkinnedObj::~SkinnedObj()
{
    mSkinBuffer->Release();
}

void SkinnedObj::Draw()
{
    mGraphics->UploadBuffer(mSkinBuffer, &mSkinConstants, sizeof(mSkinConstants));
    mGraphics->GetDeviceContext()->VSSetConstantBuffers(Graphics::CONSTANT_BUFFER_SKINNING, 1, &mSkinBuffer);
    RenderObj::Draw();
}