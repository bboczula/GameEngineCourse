#include "LightObject.h"

Sapphire::LightObject::LightObject(float x, float y, float z) : rotation(0.0f, 0.0f, 0.0f)
{
	position = { x, y, z };
	position.Normalize();
}

void Sapphire::LightObject::RotateX(float angleInDegrees)
{
	rotation.x = angleInDegrees;
	DirectX::SimpleMath::Matrix rotationX = DirectX::SimpleMath::Matrix::CreateRotationX(angleInDegrees * 3.1415 / 180.0f);
	position = position.Transform(position, rotationX);
	position.Normalize();
}

void Sapphire::LightObject::Rotate(float rx, float ry, float rz)
{
	rotation.x = rx;
	rotation.y = ry;
	rotation.z = rz;
	DirectX::SimpleMath::Matrix rotationX = DirectX::SimpleMath::Matrix::CreateRotationX(rx * 3.1415 / 180.0f);
	DirectX::SimpleMath::Matrix rotationY = DirectX::SimpleMath::Matrix::CreateRotationY(ry * 3.1415 / 180.0f);
	DirectX::SimpleMath::Matrix rotationZ = DirectX::SimpleMath::Matrix::CreateRotationZ(rz * 3.1415 / 180.0f);
	auto rotation = rotationX * rotationY * rotationZ;
	position = position.Transform(position, rotation);
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

float Sapphire::LightObject::GetRotationY()
{
	return rotation.y;
}

float Sapphire::LightObject::GetRotationZ()
{
	return rotation.z;
}
