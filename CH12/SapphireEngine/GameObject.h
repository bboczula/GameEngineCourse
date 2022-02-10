#pragma once

//#include "../SapphireMath/SapphireMath.h"
#include "DX12Geometry.h"
#include "SimpleMath.h"

#include <Windows.h>

namespace Sapphire
{
	class GameObject
	{
		friend class Renderer;
	public:
		virtual void Initialize();
		virtual void Update(float deltaTime);
		GameObject();
		~GameObject();
		DirectX::SimpleMath::Vector3* vertices;
		UINT numOfVertices;
	private:
		DX12Geometry* geometry;
		// Here you have to have the world matrix
	};
}