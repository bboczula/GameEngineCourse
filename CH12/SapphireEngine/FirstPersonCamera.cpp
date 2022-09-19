#include "FirstPersonCamera.h"

Sapphire::FirstPersonCamera::FirstPersonCamera(float aspectRatio) : Camera(aspectRatio)
{
}

Sapphire::FirstPersonCamera::~FirstPersonCamera()
{
}

void Sapphire::FirstPersonCamera::MoveZ(float delta)
{
	position.z += delta;
	target.z += delta;
}

void Sapphire::FirstPersonCamera::StrafeX(float delta)
{
	position.x += delta;
	target.x += delta;
}

void Sapphire::FirstPersonCamera::Yaw(float angle)
{
	pitchYawRoll.y += angle * 3.1415f / 180.0f;
	DirectX::SimpleMath::Matrix cameraRotation = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(pitchYawRoll);
	RecalculateBasicVectors(cameraRotation);
}

void Sapphire::FirstPersonCamera::Pitch(float angle)
{
	pitchYawRoll.x += angle * 3.1415f / 180.0f;
	DirectX::SimpleMath::Matrix cameraRotation = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(pitchYawRoll);
	RecalculateBasicVectors(cameraRotation);
}