#pragma once

#include "../SapphireEngine/GameObject.h"
#include "../SapphireEngine/SimpleMath.h"

class StaticModel : public Sapphire::GameObject
{
public:
    StaticModel()
    {
    }
    ~StaticModel()
    {
        // How to make it automatically called in GameObject
    }
    void Initialize() override
    {
        //SetScale(0.5f, 0.5f, 0.5f);
    }
    void Update(float deltaTime) override
    {
    }
};