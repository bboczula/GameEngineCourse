#pragma once

#include "Camera.h"

namespace Sapphire
{
	class FirstPersonCamera : public Camera
	{
	public:
		FirstPersonCamera(float aspectRatio);
		~FirstPersonCamera();
		void MoveZ(float delta);
		void StrafeX(float delta);
		void Yaw(float angle);
		void Pitch(float angle);
	private:

	};
}