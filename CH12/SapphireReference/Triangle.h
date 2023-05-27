#pragma once

#include "../SapphireEngine/GameObject.h"
#include "../SapphireEngine/SimpleMath.h"

class Triangle : public Sapphire::GameObject
{
public:
    Triangle() : myRotation(0.0f), GameObject("Triangle")
    {
    }
    ~Triangle()
    {
        // How to make it automatically called in GameObject
    }
    void Initialize() override
    {
        SetScale(0.5f, 0.5f, 0.5f);
    }
    void Update(float deltaTime) override
    {
        // Here you can do your own update
        if (++myRotation == 360.0f)
        {
            myRotation = 0.0f;
        }
        SetRotate(0.0f, myRotation, 0.0f);

        // Remember to call the base class update
        GameObject::Update(deltaTime);
    }

    float myRotation;
};