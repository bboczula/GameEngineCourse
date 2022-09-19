#include "OrthographicCamera.h"

Sapphire::OrthographicCamera::OrthographicCamera() : Camera(0.0f)
{
}

Sapphire::OrthographicCamera::~OrthographicCamera()
{
}

void Sapphire::OrthographicCamera::CalculateProjectionMatrix()
{
	//projection = projection.CreatePerspectiveFieldOfView(FOV_IN_RADIANS, aspectRatio, NEAR_PLANE, FAR_PLANE);
	projection = projection.CreateOrthographic(10.0f, 10.0f, 1.0f, 1000.0f);
	projection = projection.Transpose();
}

void Sapphire::OrthographicCamera::CalculateViewProjectionMatrix()
{
	view = view.CreateLookAt(position, target, up);
	projection = projection.CreateOrthographic(10.0f, 10.0f, 1.0f, 1000.0f);
	viewProjection = view * projection;
	viewProjection = viewProjection.Transpose();
}

void Sapphire::OrthographicCamera::DoIt()
{
	Sapphire::Logger::GetInstance().Log("DO IT\n");
	//DirectX::SimpleMath::Matrix cameraRotation = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll({ -90.0f * 3.1415f / 180.0f, 0.0f, 0.0f });
	//RecalculateBasicVectors(cameraRotation);
	DirectX::SimpleMath::Matrix cameraRotation = DirectX::SimpleMath::Matrix::CreateRotationX(-90.0f * 3.1415f / 180.0f);
	target = DirectX::SimpleMath::Vector3::Transform(DEFAULT_FORWARD, cameraRotation);
	up = DirectX::SimpleMath::Vector3::Transform(DEFAULT_UP, cameraRotation);
	LogInfo();
}
