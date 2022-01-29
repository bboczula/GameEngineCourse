#pragma once

#include "../SapphireEngine/GameObject.h"
#include "../SapphireEngine/SimpleMath.h"
#include "../SapphireEngine/SimpleMath.inl"

class Triangle : public Sapphire::GameObject
{
public:
    Triangle()
    {
        vertices = new DirectX::SimpleMath::Vector3[3]{ { 0.00f,  0.75f, 0.00f }, { 0.50f, -0.75f, 0.00f }, { -0.50f, -0.75f, 0.00f } };
        numOfVertices = 3;
    }
    ~Triangle()
    {
        // How to make it automatically called in GameObject
    }
};