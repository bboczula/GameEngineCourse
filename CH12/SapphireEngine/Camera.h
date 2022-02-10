#pragma once

#include "SimpleMath.h"

namespace Sapphire
{
	class Camera
	{
	public:
		Camera();
		~Camera();
		DirectX::SimpleMath::Matrix view;
		DirectX::SimpleMath::Matrix projection;
	private:

	};
}