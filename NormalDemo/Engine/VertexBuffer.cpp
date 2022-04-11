#include "stdafx.h"
#include "VertexBuffer.h"
#include "Graphics.h"

VertexBuffer::VertexBuffer(Graphics* graphics,
    const void* vertexData, uint32_t vertexCount, uint32_t vertexStride,
    const void* indexData, uint32_t indexCount, uint32_t indexStride
)
    : mVertStride(vertexStride)
    , mIndexCount(indexCount)
    , mGraphics(graphics)
{
    mVertexBuffer = mGraphics->CreateGraphicsBuffer(vertexData, vertexCount * vertexStride, D3D11_BIND_VERTEX_BUFFER, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
    mIndexBuffer = mGraphics->CreateGraphicsBuffer(indexData, indexCount * indexStride, D3D11_BIND_INDEX_BUFFER, D3D11_CPU_ACCESS_WRITE, D3D11_USAGE_DYNAMIC);
    if (indexStride == 1)
    {
        mIndexFormat = DXGI_FORMAT_R8_UINT;
    }
    else if (indexStride == 2)
    {
        mIndexFormat = DXGI_FORMAT_R16_UINT;
    }
    else
    {
        DbgAssert(indexStride == 4, "indexStride must be 2 or 4");
        mIndexFormat = DXGI_FORMAT_R32_UINT;
    }
}

VertexBuffer::~VertexBuffer()
{
    mVertexBuffer->Release();
    mIndexBuffer->Release();
}

void VertexBuffer::SetActive() const
{
    uint32_t offsets = 0;
    mGraphics->GetDeviceContext()->IASetVertexBuffers(0, 1, &mVertexBuffer, &mVertStride, &offsets);
    mGraphics->GetDeviceContext()->IASetIndexBuffer(mIndexBuffer, mIndexFormat, 0);
}

void VertexBuffer::Draw() const
{
    mGraphics->GetDeviceContext()->DrawIndexed(mIndexCount, 0, 0);
}

