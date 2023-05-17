#pragma once

#include "SimpleMath.h"

namespace Sapphire
{
	class LightObject
	{
	public:
		LightObject(float x, float y, float z);
		void RotateX(float angleInDegrees);
		void Rotate(float rx, float ry, float rz);
		float GetPositionX();
		float GetPositionY();
		float GetPositionZ();
		float GetRotationX();
		float GetRotationY();
		float GetRotationZ();
	private:
		DirectX::SimpleMath::Vector3 position;
		DirectX::SimpleMath::Vector3 rotation;
	};
}