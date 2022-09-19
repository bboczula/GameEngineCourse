#pragma once

#include "Camera.h"

namespace Sapphire
{
	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera();
		~OrthographicCamera();
		void CalculateProjectionMatrix() override;
		void CalculateViewProjectionMatrix() override;
		void DoIt();
	private:

	};
}