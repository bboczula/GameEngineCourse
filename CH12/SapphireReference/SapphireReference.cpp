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

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR pCmdLine, _In_ int nCmdShow)
{
    const unsigned int windowWidth = 1920;
    const unsigned int windowHeight = 1080;
    Sapphire::Engine engine(windowWidth, windowHeight);

    // LoadScene
    SceneDescriptor sceneDesc;
    //sceneDesc.Load("sponza.scene");
    sceneDesc.Load("crytekSponza.scene");

    std::vector<StaticModel*> staticModels;
    for (int i = 0; i < sceneDesc.Size(); i++)
    {
        StaticModel* model = new StaticModel;
        staticModels.push_back(model);
        engine.LoadModel(model, sceneDesc.GetMeshLibraryPath(), sceneDesc.GetObjectName(i));
        engine.LoadTextureFromFile(model, sceneDesc.GetColorTexturePath(i));
        if (sceneDesc.GetBumpTexturePath(i) != "")
        {
            engine.LoadBumpMapFromFile(model, sceneDesc.GetBumpTexturePath(i));
        }
        engine.Register(model);
    }


    Sapphire::PerspectiveCamera* myCamera = new Sapphire::PerspectiveCamera(static_cast<float>(windowWidth) / static_cast<float>(windowHeight), { 0.0f, 0.0f, 0.0f });
    Sapphire::FreeCamera* camera = new Sapphire::FreeCamera(static_cast<float>(windowWidth) / static_cast<float>(windowHeight), { 0.0f, 0.0f, 0.0f });
    camera->SetCamera(myCamera);

    CameraHandler* cameraHandler = new CameraHandler(camera, engine.GetInput());
    engine.Register(cameraHandler); // This is only to ge the Update method running; maybe the line below would be enough though?
    engine.RegisterCamera(myCamera);

    engine.Run();
}
