#include "GameObject.h"

Sapphire::GameObject::GameObject() : vertices(nullptr), numOfVertices(0), geometry(nullptr)
{
}

Sapphire::GameObject::~GameObject()
{
	delete geometry;
}