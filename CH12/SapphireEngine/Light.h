#pragma once

#include "SimpleMath.h"

namespace Sapphire
{
	class Light
	{
	public:
		Light(float x, float y, float z);
		~Light();
		void RotateX(float angleInDegrees);
		float GetPositionX();
		float GetPositionY();
		float GetPositionZ();
	private:
		DirectX::SimpleMath::Vector3 position;
	};
}