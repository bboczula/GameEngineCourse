#include "GameObject.h"

Sapphire::GameObject::GameObject()
{
}

Sapphire::GameObject::~GameObject()
{
	delete geometry;
}