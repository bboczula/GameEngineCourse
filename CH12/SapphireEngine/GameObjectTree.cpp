#include "GameObjectTree.h"
#include "GameObject.h"

void Sapphire::GameObjectTree::Add(GameObject* gameObject)
{
	gameObjects.push_back(gameObject);
}

bool Sapphire::GameObjectTree::IsPresent(const std::string& name)
{
	for (int i = 0; i < gameObjects.size(); i++)
	{
		if (gameObjects[i]->name.compare(name) == 0)
		{
			return true;
		}
	}
	return false;
}

void Sapphire::GameObjectTree::Initialize()
{
	for (int i = 0; i < gameObjects.size(); i++)
	{
		gameObjects[i]->Initialize();
	}
}

void Sapphire::GameObjectTree::Update(float deltaTime)
{
	for (int i = 0; i < gameObjects.size(); i++)
	{
		gameObjects[i]->Update(deltaTime);
	}
}

Sapphire::GameObject* Sapphire::GameObjectTree::At(unsigned int index)
{
	return gameObjects[index];
}

unsigned int Sapphire::GameObjectTree::Size()
{
	return gameObjects.size();
}
