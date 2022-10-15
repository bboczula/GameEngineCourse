#pragma once

#include "../SapphireEngine/GameObject.h"
#include "../SapphireEngine/SimpleMath.h"

class DynamicModel : public Sapphire::GameObject
{
public:
    DynamicModel()
    {
    }
    ~DynamicModel()
    {
        // How to make it automatically called in GameObject
    }
    void Initialize() override
    {
        SetScale(0.5f, 0.5f, 0.5f);
    }
    void Update(float deltaTime) override
    {
        rotationY += 0.5f;
        SetRotate(0.0f, rotationY, 0.0f);
        CalculateWorldMatrix();
    }
private:
    float rotationY = 0.0f;
};