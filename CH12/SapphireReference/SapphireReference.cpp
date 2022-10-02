// SapphireReference.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include "Triangle.h"
#include "CameraHandler.h"
#include "StaticModel.h"
#include "../SapphireEngine/Engine.h"
#include "../SapphireEngine/Free.h"
#include "../SapphireEngine/Arcball.h"
#include "../SapphireEngine/PerspectiveCamera.h"

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR pCmdLine, _In_ int nCmdShow)
{
    Sapphire::Engine engine;

    // SierpinskiTriangle sierpinskiTriangle;
    // engine.Register(&sierpinskiTriangle);

    // You have to turn the camera to see this
    // Triangle triangle;
    // triangle.SetPosition(0.0f, 2.2f, 0.0f);
    // engine.LoadModel(&triangle, "monkey.obj");
    // engine.Register(&triangle);
    // 
    // Triangle triangle2;
    // triangle2.SetPosition(0.0f, -2.2f, 0.0f);
    // engine.LoadModel(&triangle2, "teapot.obj");
    // engine.Register(&triangle2);

    std::vector<std::string> objectNames = {
        "arcs_floor", "arcs_03", "object32", "object31", "pillar_cor", "pillar_flo", "object28", "object27",
        "pillar_qua", "pillar_rou", "arcs_floo0", "object23", "object21", "puillar_fl", "object19", "relief",
        "arcs_small", "holes", "outside01", "arcs_long", "parapet", "round_hole", "ceiling", "arcs_01",
        "object6", "object5", "object4", "object3", "arcs_02", "arcs_04", "walls", "windows", "doors"
    };

    std::vector<std::pair<std::string, std::string>> groupTextureMapping =
    {
        std::make_pair("arcs_floor", "sp_luk.bmp"),
        std::make_pair("arcs_03", "sp_luk.bmp"),
        std::make_pair("object32", "00_skap.bmp"),
        std::make_pair("object31", "brickWall2.bmp"), // This has no texture!
        std::make_pair("pillar_cor", "01_stub.bmp"),
        std::make_pair("pillar_flo", "01_stub.bmp"),
        std::make_pair("object28", "01_S_ba.bmp"),
        std::make_pair("object27", "01_St_kp.bmp"),
        std::make_pair("pillar_qua", "01_stub.bmp"),
        std::make_pair("pillar_rou", "x01_st.bmp"),
        std::make_pair("arcs_floo0", "sp_luk.bmp"),
        std::make_pair("object23", "00_skap.bmp"),
        std::make_pair("object21", "KAMEN-stup.bmp"),
        std::make_pair("puillar_fl", "01_stub.bmp"),
        std::make_pair("object19", "sp_luk.bmp"),
        std::make_pair("relief", "reljef.bmp"),
        std::make_pair("arcs_small", "sp_luk.bmp"),
        std::make_pair("holes", "x01_st.bmp"),
        std::make_pair("outside01", "KAMEN.bmp"),
        std::make_pair("arcs_long", "sp_luk.bmp"),
        std::make_pair("parapet", "sp_luk.bmp"),
        std::make_pair("round_hole", "sp_luk.bmp"),
        std::make_pair("ceiling", "KAMEN-stup.bmp"),
        std::make_pair("arcs_01", "sp_luk.bmp"),
        std::make_pair("object6", "00_skap.bmp"),
        std::make_pair("object5", "00_skap.bmp"),
        std::make_pair("object4", "01_St_kp.bmp"),
        std::make_pair("object3", "01_S_ba.bmp"),
        std::make_pair("arcs_02", "sp_luk.bmp"),
        std::make_pair("arcs_04", "sp_luk.bmp"),
        std::make_pair("walls", "KAMEN.bmp"),
        std::make_pair("windows", "prozor1.bmp"),
        std::make_pair("doors", "vrata_ko.bmp")
        //std::make_pair("doors", "debug_tex.bmp")
    };

    std::vector<std::pair<std::string, std::string>> groupBumpMapMapping =
    {
        std::make_pair("arcs_floor", "sp_luk-bump.bmp"),
        std::make_pair("arcs_03", "sp_luk-bump.bmp"),
        std::make_pair("object32", "bumDefaultTex.bmp"),
        std::make_pair("object31", "bumDefaultTex.bmp"),
        std::make_pair("pillar_cor", "01_STUB-bump.bmp"),
        std::make_pair("pillar_flo", "01_STUB-bump.bmp"),
        std::make_pair("object28", "bumDefaultTex.bmp"),
        std::make_pair("object27", "01_St_kp-bump.bmp"),
        std::make_pair("pillar_qua", "01_STUB-bump.bmp"),
        std::make_pair("pillar_rou", "bumDefaultTex.bmp"),
        std::make_pair("arcs_floo0", "sp_luk-bump.bmp"),
        std::make_pair("object23", "bumDefaultTex.bmp"),
        std::make_pair("object21", "bumDefaultTex.bmp"),
        std::make_pair("puillar_fl", "01_STUB-bump.bmp"),
        std::make_pair("object19", "01_St_kp-bump.bmp"),
        std::make_pair("relief", "reljef-bump.bmp"),
        std::make_pair("arcs_small", "sp_luk-bump.bmp"),
        std::make_pair("holes", "bumDefaultTex.bmp"),
        std::make_pair("outside01", "KAMEN-bump.bmp"),
        std::make_pair("arcs_long", "sp_luk-bump.bmp"),
        std::make_pair("parapet", "sp_luk-bump.bmp"),
        std::make_pair("round_hole", "sp_luk-bump.bmp"),
        std::make_pair("ceiling", "bumDefaultTex.bmp"),
        std::make_pair("arcs_01", "sp_luk-bump.bmp"),
        std::make_pair("object6", "bumDefaultTex.bmp"),
        std::make_pair("object5", "bumDefaultTex.bmp"),
        std::make_pair("object4", "01_St_kp-bump.bmp"),
        std::make_pair("object3", "bumDefaultTex.bmp"),
        std::make_pair("arcs_02", "sp_luk-bump.bmp"),
        std::make_pair("arcs_04", "sp_luk-bump.bmp"),
        std::make_pair("walls", "KAMEN-bump.bmp"),
        std::make_pair("windows", "bumDefaultTex.bmp"),
        std::make_pair("doors", "bumDefaultTex.bmp")
    };

    //std::vector<std::string> objectNames = {
    //    "Object__0"
    //};

    std::vector<StaticModel*> staticModels;

    //for (int i = 0; i < objectNames.size(); i++)
    //{
    //    StaticModel* model = new StaticModel;
    //    staticModels.push_back(model);
    //    engine.LoadModel(model, "sponzaGroup.obj3d", groupTextureMapping[i].first);
    //    //engine.LoadDefaultTexture(model);
    //    engine.LoadTextureFromFile(model, groupTextureMapping[i].second);
    //    if (groupBumpMapMapping[i].second != "")
    //    {
    //        engine.LoadBumpMapFromFile(model, groupBumpMapMapping[i].second);
    //    }
    //    // Something like:
    //    // if(groupBumpMapMpaping[i].second != null)
    //    // engine.LoadBumpMapFromFile(model, groupBumpMapMapping[i].second);
    //    //engine.LoadModel(model, "sibenik.obj", objectNames[i]);
    //    engine.Register(model);
    //}

    // Shadow Test Scene
    std::vector<std::string> shadowTestObjectNames = { "Cube", "Plane" };

    std::vector<std::pair<std::string, std::string>> shadowTestTextureMapping =
    {
        std::make_pair("Cube", "crate.bmp"),
        std::make_pair("Plane", "floor.bmp")
    };

    //for (int i = 0; i < shadowTestObjectNames.size(); i++)
    //{
    //    StaticModel* model = new StaticModel;
    //    staticModels.push_back(model);
    //    engine.LoadModel(model, "shadow_test.obj", shadowTestTextureMapping[i].first);
    //    //engine.LoadDefaultTexture(model);
    //    engine.LoadTextureFromFile(model, shadowTestTextureMapping[i].second);
    //    //engine.LoadModel(model, "sibenik.obj", objectNames[i]);
    //    engine.Register(model);
    //}

    // Bump Test Scene
    std::vector<std::string> bumpTestObjectNames = { "Cube", "Plane"};

    std::vector<std::pair<std::string, std::string>> bumpTestTextureMapping =
    {
        std::make_pair("Cube", "01_STUB.bmp"),
        std::make_pair("Plane", "01_STUB.bmp")
    };

    std::vector<std::pair<std::string, std::string>> bumpTestBumpMapping =
    {
        std::make_pair("Cube", "01_STUB-bump.bmp"),
        std::make_pair("Plane", "01_STUB-bump.bmp")
    };

    //for (int i = 0; i < bumpTestObjectNames.size(); i++)
    //{
    //    StaticModel* model = new StaticModel;
    //    staticModels.push_back(model);
    //    engine.LoadModel(model, "bump_test.obj", bumpTestTextureMapping[i].first);
    //    //engine.LoadDefaultTexture(model);
    //    engine.LoadTextureFromFile(model, bumpTestTextureMapping[i].second);
    //    if (bumpTestBumpMapping[i].second != "")
    //    {
    //        engine.LoadBumpMapFromFile(model, bumpTestBumpMapping[i].second);
    //    }
    //    engine.Register(model);
    //}

    // OSIEDLE
    std::vector<std::string> osiedleObjectNames = { "Plane_Mesh", "Monkey_Mesh" };

    std::vector<std::pair<std::string, std::string>> osiedleTextureMapping =
    {
        std::make_pair("Plane_Mesh", "01_STUB.bmp"),
        std::make_pair("Monkey_Mesh", "01_STUB.bmp")
    };

    std::vector<std::pair<std::string, std::string>> osiedleBumpMapping =
    {
        std::make_pair("Plane_Mesh", "01_STUB-bump.bmp"),
        std::make_pair("Monkey_Mesh", "01_STUB.bmp")
    };

    for (int i = 0; i < osiedleObjectNames.size(); i++)
    {
        StaticModel* model = new StaticModel;
        staticModels.push_back(model);
        engine.LoadModel(model, "monkey.obj3d", osiedleTextureMapping[i].first);
        //engine.LoadDefaultTexture(model);
        engine.LoadTextureFromFile(model, osiedleTextureMapping[i].second);
        if (osiedleBumpMapping[i].second != "")
        {
            engine.LoadBumpMapFromFile(model, osiedleBumpMapping[i].second);
        }
        engine.Register(model);
    }


    Sapphire::PerspectiveCamera* myCamera = new Sapphire::PerspectiveCamera(static_cast<float>(1280) / static_cast<float>(720), { 0.0f, 15.0f, 0.0f });
    //Sapphire::FirstPersonCamera* camera = new Sapphire::FirstPersonCamera(static_cast<float>(1280) / static_cast<float>(720));
    Sapphire::FreeCamera* camera = new Sapphire::FreeCamera(static_cast<float>(1280) / static_cast<float>(720), { 0.0f, 15.0f, 0.0f });
    //Sapphire::Arcball* camera = new Sapphire::Arcball(myCamera);
    //camera->Rotate(0.0f, 90.0f, 0.0f);

    // Bind camera to the controler
    camera->SetCamera(myCamera);

    CameraHandler* cameraHandler = new CameraHandler(camera, engine.GetInput());
    engine.Register(cameraHandler); // This is only to ge the Update method running; maybe the line below would be enough though?
    engine.RegisterCamera(myCamera);

    engine.Run();
}
