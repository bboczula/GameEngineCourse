#pragma once

#include "../SapphireMath/SapphireMath.h"
#include "DX12Geometry.h"

#include <Windows.h>

namespace Sapphire
{
	class GameObject
	{
		friend class Renderer;
	public:
		GameObject();
		~GameObject();
		Math::Point3D* vertices;
		UINT numOfVertices;
	private:
		DX12Geometry* geometry;
	};
}