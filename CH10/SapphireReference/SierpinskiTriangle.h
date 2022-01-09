#pragma once

#include "../SapphireEngine/GameObject.h"

class SierpinskiTriangle : public Sapphire::GameObject
{
public:
    SierpinskiTriangle()
    {
        calculate({ 0.00f,  0.75f, 0.00f }, { 0.50f, -0.75f, 0.00f }, { -0.50f, -0.75f, 0.00f }, 5);
        vertices = sierpinski.data();
        numOfVertices = sierpinski.size();
    }
private:
    void calculate(Sapphire::Math::Point3D a, Sapphire::Math::Point3D b, Sapphire::Math::Point3D c, int level)
    {
        if (level == 0)
        {
            sierpinski.push_back(a);
            sierpinski.push_back(b);
            sierpinski.push_back(c);
            return;
        }
        calculate(a, middlePoint(a, b), middlePoint(a, c), level - 1);
        calculate(middlePoint(a, b), b, middlePoint(b, c), level - 1);
        calculate(middlePoint(a, c), middlePoint(b, c), c, level - 1);
    }
    Sapphire::Math::Point3D middlePoint(Sapphire::Math::Point3D a, Sapphire::Math::Point3D b)
    {
        return Sapphire::Math::Point3D(((a.x + b.x) / 2.0f), ((a.y + b.y) / 2.0f), 0.0f);
    }
    std::vector<Sapphire::Math::Point3D> sierpinski;
};