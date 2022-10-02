#include "OrthographicCamera.h"

Sapphire::OrthographicCamera::OrthographicCamera(DirectX::SimpleMath::Vector3 position)	: Camera()
{
	CalculateViewProjectionMatrix();
}

void Sapphire::OrthographicCamera::CalculateViewProjectionMatrix()
{
	auto view = DirectX::SimpleMath::Matrix::CreateLookAt(position, forward, up);
	auto projection = DirectX::SimpleMath::Matrix::CreateOrthographic(50.0f, 50.0f, 0.1f, 1000.0f);
	viewProjection = view * projection;
	viewProjection = viewProjection.Transpose();
}
