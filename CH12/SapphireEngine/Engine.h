#pragma once

#include <iostream>

#include "WindowApplication.h"
#include "Logger.h"
#include "GameObject.h"
#include "ModelLoaderSapphire.h"
#include "RawInput.h"
#include "ImGuiHandler.h"
#include "Camera.h"
#include "WinMouse.h"
#include "FirstPerson.h"
#include "RenderInterface.h"

#include "../ModelLoader/ModelLoader.h"
#include "../ModelLoader/Image.h"

#include <vector>

#define DEFAULT_WINDOW_WIDTH 1280
#define DEFAULT_WINDOW_HEIGHT 720

namespace Sapphire
{
	class Renderer;
	class LightObject;

	class Engine : public WindowApplication
	{
	public:
		Engine(UINT width = DEFAULT_WINDOW_WIDTH, UINT height = DEFAULT_WINDOW_HEIGHT);
		~Engine();
		void Register(GameObject* gameObject);
		void RegisterCamera(Camera* camera);
		void RegisterLight(LightObject* light);
		void LoadModel(GameObject* gameObject, const std::string& filePath, std::string sceneName, const std::string& groupName);
		void LoadDefaultTexture(GameObject* gameObject);
		void LoadTextureFromFile(GameObject* gameObject, std::string sceneName, std::string filePath);
		void LoadBumpMapFromFile(GameObject* gameObject, std::string sceneName, std::string filePath);
		RawInput* GetInput();
	private:
		void Initialize() override;
		void Tick() override;
		void ReportLiveObjects();
		RenderInterface* renderer;
		Renderer* newRenderer;
		RawInput* input;
		WinMouse* winMouse;
		ImGuiHandler* imGuiHandler;
		std::vector<GameObject*> gameObjects;
		std::vector<LightObject*> lightObjects;
		ModelLoader* modelLoader;
		bool isPaused;
	};
}