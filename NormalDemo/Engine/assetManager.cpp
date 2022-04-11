#include "stdafx.h"
#include "assetManager.h"
#include "Graphics.h"

AssetManager::AssetManager(Graphics* pGraphics)
    : mGraphics(pGraphics)
    , mShaderCache(this)
    , mTextureCache(this)
    , mMeshCache(this)
    , mSkeletonCache(this)
    , mAnimCache(this)
{
}

AssetManager::~AssetManager()
{
}

void AssetManager::Clear()
{
    mShaderCache.Clear();
    mTextureCache.Clear();
    mMeshCache.Clear();
    mSkeletonCache.Clear();
    mAnimCache.Clear();
}