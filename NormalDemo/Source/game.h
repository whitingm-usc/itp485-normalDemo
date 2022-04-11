#pragma once
#include "assetManager.h"
#include "Graphics.h"
#include "Job.h"
#include "Physics.h"

class Camera;
class RenderObj;
class Shader;
class Texture;

class Game
{
public:
    enum { MAX_POINT_LIGHTS = 8 };
    struct PointLightData
    {
        Vector3 diffuseColor;
        float pad0;

        Vector3 specularColor;
        float pad1;

        Vector3 position;
        float specularPower;

        float innerRadius;
        float outerRadius;
        bool isEnabled;
        bool pad2, pad3, pad4;
        float pad5;
    };

    struct LightingConstants
    {
        Vector3	c_ambient;
        float pad0;
        PointLightData c_pointLight[MAX_POINT_LIGHTS];
    };

    Game();
    ~Game();

    void Init(HWND hWnd, float width, float height);
    void Shutdown();
	void Update(float deltaTime);
    void RenderFrame();

	void OnKeyDown(uint32_t key);
	void OnKeyUp(uint32_t key);
	bool IsKeyHeld(uint32_t key) const;

    // Lighting Access
    PointLightData* AllocateLight();
    void FreeLight(PointLightData* pLight);
    void SetAmbientLight(const Vector3& color) { mLightData.c_ambient = color;  }
    const Vector3& GetAmbientLight() const { return mLightData.c_ambient;  }

    AssetManager* GetAssetManager() { return &mAssets; }

    void AddJob(JobManager::Job* pJob) { mJobs.AddJob(pJob); }

    Camera* GetCamera() { return mCamera; }
    Physics* GetPhysics() { return &mPhysics; }

private:
	std::unordered_map<uint32_t, bool> m_keyIsHeld;
    Graphics mGraphics;
    JobManager mJobs;
    Physics mPhysics;

    Camera* mCamera;
    AssetManager mAssets;

    std::vector<RenderObj*> mRenderObj;

    // lighting data
    LightingConstants mLightData;
    ID3D11Buffer* mLightBuffer;

	bool LoadLevel(const WCHAR* fileName);
};