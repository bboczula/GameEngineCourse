#include "Camera.h"

Sapphire::Camera::Camera()
{
	DirectX::SimpleMath::Vector3 eye(0.0f, 0.0f, -8.0f);
	DirectX::SimpleMath::Vector3 target(0.0f, 0.0f, 0.0f);
	DirectX::SimpleMath::Vector3 up(0.0f, 1.0f, 0.0f);
	view = view.CreateLookAt(eye, target, up);

	projection = projection.CreatePerspectiveFieldOfView(DirectX::XMConvertToRadians(36.0f), 1280.0f / 720.0f, 0.01f, 1000.0f);
}

Sapphire::Camera::~Camera()
{
}