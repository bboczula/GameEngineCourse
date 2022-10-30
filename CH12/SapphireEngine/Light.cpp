#include "Light.h"

Sapphire::Light::Light(float x, float y, float z) : rotation(0.0f, 0.0f, 0.0f)
{
	position = { x, y, z };
	position.Normalize();
}

Sapphire::Light::~Light()
{
}

void Sapphire::Light::RotateX(float angleInDegrees)
{
	rotation.x = angleInDegrees;// *3.1415 / 180.0f;
	DirectX::SimpleMath::Matrix rotationX = DirectX::SimpleMath::Matrix::CreateRotationX(angleInDegrees * 3.1415 / 180.0f);
	position = position.Transform(position, rotationX);
	position.Normalize();
}

float Sapphire::Light::GetPositionX()
{
	return position.x;
}

float Sapphire::Light::GetPositionY()
{
	return position.y;
}

float Sapphire::Light::GetPositionZ()
{
	return position.z;
}

float Sapphire::Light::GetRotationX()
{
	return rotation.x;
}