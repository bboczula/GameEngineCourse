#pragma once

#include "../SapphireEngine/GameObject.h"

class Triangle : public Sapphire::GameObject
{
public:
    Triangle()
    {
        vertices = new Sapphire::Math::Point3D[3]{ { 0.00f,  0.75f, 0.00f }, { 0.50f, -0.75f, 0.00f }, { -0.50f, -0.75f, 0.00f } };
        numOfVertices = 3;
    }
};