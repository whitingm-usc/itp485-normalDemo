#pragma once
#include "Animation.h"
#include "mesh.h"
#include "Shader.h"
#include "Skeleton.h"
#include "Texture.h"
#include "assetCache.h"

class Graphics;

class AssetManager
{
public:
    AssetManager(Graphics* pGraphics);
    ~AssetManager();

    void Clear();

    Graphics* GetGraphics() { return mGraphics; }

    Shader* GetShader(const std::wstring& shaderName) { return mShaderCache.Get(shaderName); }
    void SetShader(const std::wstring& shaderName, Shader* pShader) { mShaderCache.Cache(shaderName, pShader); }

    Texture* LoadTexture(const std::wstring& fileName) { return mTextureCache.Load(fileName); }

    Mesh* GetMesh(const std::wstring& meshName) { return mMeshCache.Get(meshName); }
    void SetMesh(const std::wstring& meshName, Mesh* pMesh) { mMeshCache.Cache(meshName, pMesh); }
    Mesh* LoadMesh(const std::wstring& fileName) { return mMeshCache.Load(fileName); }

    Skeleton* LoadSkeleton(const std::wstring& fileName) { return mSkeletonCache.Load(fileName); }
    Animation* LoadAnimation(const std::wstring& fileName) { return mAnimCache.Load(fileName); }

private:
    Graphics* mGraphics;

    AssetCache<Shader> mShaderCache;
    AssetCache<Texture> mTextureCache;
    AssetCache<Mesh> mMeshCache;
    AssetCache<Skeleton> mSkeletonCache;
    AssetCache<Animation> mAnimCache;
};
