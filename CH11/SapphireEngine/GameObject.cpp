#include "GameObject.h"

void Sapphire::GameObject::Initialize()
{
}

void Sapphire::GameObject::Update(float deltaTime)
{
}

Sapphire::GameObject::GameObject() : vertices{ nullptr }, numOfVertices{ 0 }, geometry{ nullptr }
{
}

Sapphire::GameObject::~GameObject()
{
	delete geometry;
}