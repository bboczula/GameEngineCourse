#pragma once

#include <iostream>

#include "WindowApplication.h"
#include "Logger.h"
#include "Renderer.h"

#define DEFAULT_WINDOW_WIDTH 1280
#define DEFAULT_WINDOW_HEIGHT 720

namespace Sapphire
{
    class Engine : public WindowApplication
    {
    public:
        Engine(UINT width = DEFAULT_WINDOW_WIDTH, UINT height = DEFAULT_WINDOW_HEIGHT);
        ~Engine();
    private:
        void Initialize() override;
        void Tick() override;
        void ReportLiveObjects();
        Renderer* renderer;
    };
}