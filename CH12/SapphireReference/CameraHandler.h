#pragma once

#include "../SapphireEngine/GameObject.h"
#include "../SapphireEngine/FirstPersonCamera.h"
#include "../SapphireEngine/RawInput.h"

class CameraHandler : public Sapphire::GameObject
{
public:
	CameraHandler(Sapphire::FirstPersonCamera* camera, RawInput* input);
	~CameraHandler();
    void Initialize() override
    {
    }
    void Update(float deltaTime) override
    {
        if (input->isKeyDown(0x53))
        {
            camera->MoveZ(0.1f);
        }
        if (input->isKeyDown(0x57))
        {
            camera->MoveZ(-0.1f);
        }
        if (input->isKeyDown(0x44))
        {
            camera->StrafeX(0.1f);
        }
        if (input->isKeyDown(0x41))
        {
            camera->StrafeX(-0.1f);
        }

        // For now this have to be after position changes :/ Maybe I have to add some PostUpdate virtual method or something
        camera->Yaw(input->getMouseXDelta() * -0.5f);
        camera->Pitch(input->getMouseYDelta() * 0.5f);
    }
private:
    Sapphire::FirstPersonCamera* camera;
    RawInput* input;
};

CameraHandler::CameraHandler(Sapphire::FirstPersonCamera* camera, RawInput* input) : camera(camera), input(input)
{
}

CameraHandler::~CameraHandler()
{
}