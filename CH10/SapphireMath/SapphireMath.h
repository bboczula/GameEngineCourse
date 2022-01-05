#pragma once

namespace Sapphire
{
	namespace Math
	{
		struct Point3D
		{
			Point3D() : x(0.0f), y(0.0f), z(0.0f)
			{
			}
			Point3D(float _x, float _y, float _z) : x(_x), y(_y), z(_z)
			{
			}
			float x, y, z;
		};
	}
}