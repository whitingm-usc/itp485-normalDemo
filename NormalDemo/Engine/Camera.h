#pragma once
#include "engineMath.h"

class Graphics;

class Camera
{
public:
    Camera(Graphics* pGraphics);
    virtual ~Camera();
    void SetActive();

    Matrix4 mView;
    Matrix4 mProj;

protected:
    struct PerCameraConstants
    {
        Matrix4 c_viewProj;
        Vector3 c_cameraPosition;
        float pad;
    };

    Graphics* mGraphics;
    PerCameraConstants mCameraData;
    ID3D11Buffer* mCameraBuffer;
};