#include "GameObject.h"

Sapphire::GameObject::GameObject() : numOfVertices{ 0 }, indices{ nullptr }, numOfIndices{ 0 }, texture{ nullptr },
rotation{ 0.0f, 0.0f, 0.0f }, scale{ 1.0f, 1.0f, 1.0f }, translation{ 0.0f, 0.0f, 0.0f }//, isVisible { true }
{
	CalculateWorldMatrix();
}

Sapphire::GameObject::~GameObject()
{
}

void Sapphire::GameObject::Initialize()
{
}

void Sapphire::GameObject::Update(float deltaTime)
{
	CalculateWorldMatrix();
}

void Sapphire::GameObject::CalculateWorldMatrix()
{
	DirectX::SimpleMath::Matrix scaleMatrix;
	scaleMatrix = scaleMatrix.CreateScale(DirectX::SimpleMath::Vector3( metaScaleX.value, metaScaleY.value, metaScaleZ.value ));

	DirectX::SimpleMath::Matrix rotationMatrix;
	rotationMatrix = rotationMatrix.CreateRotationX(rotation.x * 3.1415 / 180.0f);
	rotationMatrix *= rotationMatrix.CreateRotationY(rotation.y * 3.1415 / 180.0f);
	rotationMatrix *= rotationMatrix.CreateRotationZ(rotation.z * 3.1415 / 180.0f);

	DirectX::SimpleMath::Matrix translateMatrix;
	translateMatrix = translateMatrix.CreateTranslation(translation);

	world = scaleMatrix * rotationMatrix * translateMatrix;
	world = world.Transpose();
}

void Sapphire::GameObject::SetRotate(float x, float y, float z)
{
	rotation = { x, y, z };
}

void Sapphire::GameObject::SetScale(float x, float y, float z)
{
	metaScaleX.value = x;
	metaScaleY.value = y;
	metaScaleZ.value = z;
	scale = { x, y, z };
}

void Sapphire::GameObject::SetPosition(float x, float y, float z)
{
	translation = { x, y, z };
}

//bool Sapphire::GameObject::GetIsVisible()
//{
//	return isVisible;
//}
//
//void Sapphire::GameObject::SetIsVisible(bool isVisible)
//{
//	this->isVisible = isVisible;
//}

DirectX::SimpleMath::Vector3 Sapphire::GameObject::GetTranslation()
{
	return translation;
}

void Sapphire::GameObject::SetTranslation(float x, float y, float z)
{
	translation = { x, y, z };
	//translation.x = x;
}

DirectX::SimpleMath::Vector3 Sapphire::GameObject::GetScale()
{
	return scale;
}

DirectX::SimpleMath::Vector3 Sapphire::GameObject::GetRotate()
{
	return rotation;
}
