#pragma once

#include "../SapphireEngine/GameObject.h"
#include "../SapphireEngine/SimpleMath.h"

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
    void calculate(DirectX::SimpleMath::Vector3 a, DirectX::SimpleMath::Vector3 b, DirectX::SimpleMath::Vector3 c, int level)
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
    DirectX::SimpleMath::Vector3 middlePoint(DirectX::SimpleMath::Vector3 a, DirectX::SimpleMath::Vector3 b)
    {
        return DirectX::SimpleMath::Vector3(((a.x + b.x) / 2.0f), ((a.y + b.y) / 2.0f), ((a.z + b.z) / 2.0f));
    }
    std::vector<DirectX::SimpleMath::Vector3> sierpinski;
};