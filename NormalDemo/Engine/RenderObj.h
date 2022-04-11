#pragma once
#include "engineMath.h"
#include "Graphics.h"

class Component;
class Mesh;

class RenderObj
{
public:
    struct PerObjectConstants
    {
        Matrix4 c_modelToWorld;
    };

    RenderObj(Graphics* graphics, const Mesh* pMesh);
    virtual ~RenderObj();
    virtual void Draw();
    virtual void Update(float deltaTime);
    void AddComponent(Component* pComp) { mComponents.emplace_back(pComp); }
    
    PerObjectConstants mObjectData;

protected:
    Graphics* mGraphics;
    const Mesh* mMesh;
    ID3D11Buffer* mObjectBuffer;
    std::vector<Component*> mComponents;
};