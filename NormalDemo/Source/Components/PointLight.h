#pragma once
#include "Components\Component.h"
#include "game.h"

class PointLight : public Component
{
public:
    PointLight(Game* pGame, RenderObj* pObj);
    ~PointLight() override;
    void LoadProperties(const rapidjson::Value& properties) override;
    void Update(float deltaTime) override;

private:
    Game* mGame;
    Game::PointLightData* mLight;
};
