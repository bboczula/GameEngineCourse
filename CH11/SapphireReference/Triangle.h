#pragma once

#include "../SapphireEngine/GameObject.h"
#include "../SapphireEngine/SimpleMath.h"

class Triangle : public Sapphire::GameObject
{
public:
    Triangle()
    {
        vertices = new DirectX::SimpleMath::Vector3[3]{ { 0.00f,  2.00f, 0.00f }, { 2.00f, 0.00f, 0.00f }, { -2.00f, -0.00f, 0.00f } };
        numOfVertices = 3;
    }
    ~Triangle()
    {
        // How to make it automatically called in GameObject
    }
};