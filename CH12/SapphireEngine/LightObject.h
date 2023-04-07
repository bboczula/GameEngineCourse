#pragma once

#include "SimpleMath.h"

namespace Sapphire
{
	class LightObject
	{
	public:
		LightObject(float x, float y, float z);
		void RotateX(float angleInDegrees);
		float GetPositionX();
		float GetPositionY();
		float GetPositionZ();
		float GetRotationX();
	private:
		DirectX::SimpleMath::Vector3 position;
		DirectX::SimpleMath::Vector3 rotation;
	};
}