#include "GameObject.h"

Sapphire::GameObject::GameObject() : numOfVertices{ 0 }, indices{ nullptr }, numOfIndices{ 0 }, texture{ nullptr },
rotation{ 0.0f, 0.0f, 0.0f }, scale{ 1.0f, 1.0f, 1.0f }, translation{ 0.0f, 0.0f, 0.0f }, isVisible { true }
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
	scaleMatrix = scaleMatrix.CreateScale(scale);

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
	scale = { x, y, z };
}

void Sapphire::GameObject::SetPosition(float x, float y, float z)
{
	translation = { x, y, z };
}

bool Sapphire::GameObject::GetIsVisible()
{
	return isVisible;
}

void Sapphire::GameObject::SetIsVisible(bool isVisible)
{
	this->isVisible = isVisible;
}
