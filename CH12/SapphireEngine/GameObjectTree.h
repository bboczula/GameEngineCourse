#pragma once

#include <vector>
#include <string>

namespace Sapphire
{
	class GameObject;

	class GameObjectTree
	{
	public:
		/* Add a new pointer to existing Game Object */
		void Add(GameObject* gameObject);
		/* Check if the tree already contains Game Object with given name */
		bool IsPresent(const std::string& name);
		void Initialize();
		void Update(float deltaTime);
		GameObject* At(unsigned int index);
		GameObject* GetByName(const std::string& name);
		unsigned int Size();
	private:
		std::vector<GameObject*> gameObjects;
	};

	template<typename T>
	class GameObjectT
	{
	};
}