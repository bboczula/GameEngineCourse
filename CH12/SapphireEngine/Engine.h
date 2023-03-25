#pragma once

#include <iostream>

#include "WindowApplication.h"
#include "Logger.h"
#include "GameObject.h"
#include "ModelLoaderSapphire.h"
#include "RawInput.h"
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
    class Engine : public WindowApplication
    {
    public:
        Engine(UINT width = DEFAULT_WINDOW_WIDTH, UINT height = DEFAULT_WINDOW_HEIGHT);
        ~Engine();
        void Register(GameObject* gameObject);
        void RegisterCamera(Camera* camera);
        void LoadModel(GameObject* gameObject, const std::string& filePath, const std::string& groupName);
        void LoadDefaultTexture(GameObject* gameObject);
        void LoadTextureFromFile(GameObject* gameObject, std::string filePath);
        void LoadBumpMapFromFile(GameObject* gameObject, std::string filePath);
        RawInput* GetInput();
    private:
        void Initialize() override;
        void Tick() override;
        void ReportLiveObjects();
        RenderInterface* renderer;
        Renderer* newRenderer;
        RawInput* input;
        WinMouse* winMouse;
        std::vector<GameObject*> gameObjects;
        ModelLoader* modelLoader;
    };
}