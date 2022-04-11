#include "stdafx.h"
#include "Character.h"
#include "Animation.h"
#include "game.h"
#include "jsonUtil.h"
#include "RenderObj.h"
#include "Skeleton.h"
#include "SkinnedObj.h"
#include "stringUtil.h"


Character::Character(Game* pGame, SkinnedObj* pObj)
    : Component(pObj)
    , mGame(pGame)
    , mSkeleton(nullptr)
    , mCurrentAnim(nullptr)
    , mAnimationTime(0.0f)
    , mAnimJob(this)
{
    mSkinnedObj = pObj;
}

void Character::LoadProperties(const rapidjson::Value& properties)
{
    Component::LoadProperties(properties);
    std::string skeleton;
    if (GetStringFromJSON(properties, "skeleton", skeleton))
    {
        std::wstring skeletonwstr;
        StringUtil::String2WString(skeletonwstr, skeleton);
        mSkeleton = mGame->GetAssetManager()->LoadSkeleton(skeletonwstr);
    }

    const rapidjson::Value& anims = properties["animations"];
    if (anims.IsArray())
    {
        for (rapidjson::SizeType i = 0; i < anims.Size(); ++i)
        {
            if (anims[i].IsArray() && anims[i].Size() == 2)
            {
                std::string animType = anims[i][0].GetString();
                std::string animName = anims[i][1].GetString();
                std::wstring animName_w;
                StringUtil::String2WString(animName_w, animName);
                mAnims[animType] = mGame->GetAssetManager()->LoadAnimation(animName_w);
            }
        }
    }
}

bool Character::SetAnim(const std::string& animName)
{
    auto find = mAnims.find(animName);
    if (find != mAnims.end())
    {
        mCurrentAnim = find->second;
        mAnimationTime = 0.0f;
        return true;
    }
    return false;
}

void Character::UpdateAnim(float deltaTime)
{
    if (nullptr != mCurrentAnim && nullptr != mSkeleton && nullptr != mSkinnedObj)
    {
        mAnimationTime += deltaTime;
        while (mAnimationTime >= mCurrentAnim->GetLength())
        {
            mAnimationTime -= mCurrentAnim->GetLength();
        }
        mGame->AddJob(&mAnimJob);
    }
}

void Character::DoAnimation()
{
    if (nullptr != mCurrentAnim && nullptr != mSkeleton && nullptr != mSkinnedObj)
    {
        std::vector<Matrix4> outPoses;
        mCurrentAnim->GetGlobalPoseAtTime(outPoses, mSkeleton, mAnimationTime);
        for (int i = 0; i < mSkeleton->GetNumBones(); ++i)
        {
            mSkinnedObj->mSkinConstants.c_skinMatrix[i] = mSkeleton->GetGlobalInvBindPoses()[i] * outPoses[i];
        }
    }
}

void Character::Update(float deltaTime)
{
    if (nullptr == mCurrentAnim)
        SetAnim("idle");
    UpdateAnim(deltaTime);
}
