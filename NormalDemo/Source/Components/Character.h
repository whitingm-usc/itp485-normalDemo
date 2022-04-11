#pragma once
#include "Components\Component.h"
#include "Job.h"

class Animation;
class Game;
class RenderObj;
class Skeleton;
class SkinnedObj;

class Character : public Component
{
public:
    Character(Game* pGame, SkinnedObj* pObj);
    void LoadProperties(const rapidjson::Value& properties) override;
    bool SetAnim(const std::string& animName);
    void UpdateAnim(float deltaTime);
    void Update(float deltaTime) override;

protected:
    Game* mGame;
    SkinnedObj* mSkinnedObj;
    Skeleton* mSkeleton;
    std::unordered_map<std::string, const Animation*> mAnims;
    const Animation* mCurrentAnim;
    float mAnimationTime;

    // Animation Job
    class AnimJob : public JobManager::Job
    {
    public:
        AnimJob(Character* pChar) : mChar(pChar) {}
        virtual void DoIt() override
        {
            mChar->DoAnimation();
        }

    private:
        Character* mChar;
    } mAnimJob;
    void DoAnimation();
};