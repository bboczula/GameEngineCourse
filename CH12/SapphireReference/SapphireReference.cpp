// SapphireReference.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include "Triangle.h"
#include "CameraHandler.h"
#include "StaticModel.h"
#include "SceneDescriptor.h"
#include "../SapphireEngine/Engine.h"
#include "../SapphireEngine/Free.h"
#include "../SapphireEngine/Arcball.h"
#include "../SapphireEngine/PerspectiveCamera.h"
#include "../SapphireEngine/InputFile.h"
#include "../SapphireEngine/LightObject.h"

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR pCmdLine, _In_ int nCmdShow)
{
    const unsigned int windowWidth = 1920;
    const unsigned int windowHeight = 1080;
    Sapphire::Engine engine(windowWidth, windowHeight);

    // LoadScene
    SceneDescriptor sceneDesc;
    sceneDesc.Load("crytekSponza.scene");
    //sceneDesc.Load("floor.scene");
    //sceneDesc.Load("house.scene");
    //sceneDesc.Load("table_and_chairs.scene");
    auto sceneName = sceneDesc.GetSceneName();

    std::vector<StaticModel*> staticModels;
    for (int i = 0; i < sceneDesc.Size(); i++)
    {
        StaticModel* model = new StaticModel;
        staticModels.push_back(model);
        ObjectData temp = sceneDesc.GetObjectData(i);
        engine.LoadModel(model, sceneDesc.GetMeshLibraryPath(), sceneName, temp.name.Get());

        engine.LoadTextureFromFile(model, sceneName, temp.colorTextureName.Get());
        if (temp.bumpTextureName.Get() != "null-texture")
        {
            std::string bumpTextureExtension = temp.bumpTextureName.Get().substr(temp.bumpTextureName.Get().find(".") + 1);
            engine.LoadBumpMapFromFile(model, sceneName, temp.bumpTextureName.Get());
        }
        engine.Register(model);
    }


    Sapphire::PerspectiveCamera* myCamera = new Sapphire::PerspectiveCamera(static_cast<float>(windowWidth) / static_cast<float>(windowHeight), { 0.0f, 0.0f, 0.0f });
    Sapphire::FreeCamera* camera = new Sapphire::FreeCamera(static_cast<float>(windowWidth) / static_cast<float>(windowHeight), { 0.0f, 0.0f, 0.0f });
    camera->SetCamera(myCamera);

    CameraHandler* cameraHandler = new CameraHandler(camera, engine.GetInput());
    engine.Register(cameraHandler); // This is only to ge the Update method running; maybe the line below would be enough though?
    engine.RegisterCamera(myCamera);

    // Light
    Sapphire::LightObject* sunlight = new Sapphire::LightObject(0.0f, 1.0f, 0.0f);
    sunlight->RotateX(0.0f);
    engine.RegisterLight(sunlight);

    engine.Run();
}
