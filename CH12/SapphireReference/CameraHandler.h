#pragma once

#include "../SapphireEngine/GameObject.h"
#include "../SapphireEngine/Free.h"
#include "../SapphireEngine/RawInput.h"

class CameraHandler : public Sapphire::GameObject
{
public:
	CameraHandler(Sapphire::CameraControler* camera, RawInput* input);
	~CameraHandler();
    void Initialize() override
    {
    }
    void Update(float deltaTime) override
    {
        if (input->IsKeyDown(0x53))
        {
            camera->MoveForward(-0.1f);
        }
        if (input->IsKeyDown(0x57))
        {
            camera->MoveBackward(-0.1f);
        }
        if (input->IsKeyDown(0x44))
        {
            camera->MoveRight(0.1f);
        }
        if (input->IsKeyDown(0x41))
        {
            camera->MoveLeft(0.1f);
        }

        // For now this have to be after position changes :/ Maybe I have to add some PostUpdate virtual method or something
        //camera->Yaw(input->getMouseXDelta() * -0.5f);
        //camera->Pitch(input->getMouseYDelta() * 0.5f);
        camera->Rotate(input->GetMouseYDelta() * -0.5f, input->GetMouseXDelta() * -0.5f, 0.0f);
    }
private:
    Sapphire::CameraControler* camera;
    RawInput* input;
};

CameraHandler::CameraHandler(Sapphire::CameraControler* camera, RawInput* input) : camera(camera), input(input), GameObject("CameraHandler")
{
}

CameraHandler::~CameraHandler()
{
}