#include "LightObject.h"

Sapphire::LightObject::LightObject(float x, float y, float z) : rotation(0.0f, 0.0f, 0.0f)
{
	position = { x, y, z };
	position.Normalize();
}

void Sapphire::LightObject::RotateX(float angleInDegrees)
{
	rotation.x = angleInDegrees;// *3.1415 / 180.0f;
	DirectX::SimpleMath::Matrix rotationX = DirectX::SimpleMath::Matrix::CreateRotationX(angleInDegrees * 3.1415 / 180.0f);
	position = position.Transform(position, rotationX);
	position.Normalize();
}

float Sapphire::LightObject::GetPositionX()
{
	return position.x;
}

float Sapphire::LightObject::GetPositionY()
{
	return position.y;
}

float Sapphire::LightObject::GetPositionZ()
{
	return position.z;
}

float Sapphire::LightObject::GetRotationX()
{
	return rotation.x;
}
