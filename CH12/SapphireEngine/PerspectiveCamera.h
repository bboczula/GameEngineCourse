#pragma once

#include "Camera.h"

namespace Sapphire
{
	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera(float aspectRatio);
		~PerspectiveCamera();
		void CalculateProjectionMatrix() override;
	private:

	};
}