#include "RawInput.h"

void RawInput::Handle(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_INPUT:
    {
#if 1
        UINT dwSize = { 0 };
        GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));

        rawBuffer.resize(dwSize);
        GetRawInputData((HRAWINPUT)lParam, RID_INPUT, rawBuffer.data(), &dwSize, sizeof(RAWINPUTHEADER));

        // You can check for GetRawInputData return value for errors

        auto& ri = reinterpret_cast<const RAWINPUT&>(*rawBuffer.data());
        ri.data.mouse;

        if (ri.header.dwType == RIM_TYPEKEYBOARD)
        {
            // std::cout << "Keyboard: makeCode: " << raw->data.keyboard.MakeCode << ", flags: " << raw->data.keyboard.Flags << ", reserved: " << raw->data.keyboard.Reserved << ", extraInformation: " << raw->data.keyboard.ExtraInformation << ", message: " << raw->data.keyboard.Message << ", vKey: " << raw->data.keyboard.VKey << std::endl;
            if (ri.data.keyboard.Flags == 0)
            {
                virtualKeyState[ri.data.keyboard.VKey] = true;
            }
            else
            {
                virtualKeyState[ri.data.keyboard.VKey] = false;
            }
        }
        else if (ri.header.dwType == RIM_TYPEMOUSE)
        {
            //std::cout << "Mouse: usFlags: " << raw->data.mouse.usFlags << ", ulButtons: " << raw->data.mouse.ulButtons << ", usButtonFlags: " << raw->data.mouse.usButtonFlags << ", usButtonData: " << raw->data.mouse.usButtonData << ", ulRawButtons: " << raw->data.mouse.ulRawButtons << ", lastX: " << raw->data.mouse.lLastX << ", lastY: " << raw->data.mouse.lLastY << ", extraInfo: " << raw->data.mouse.ulExtraInformation << std::endl;
            auto tempDeltaX = ri.data.mouse.lLastX;
            auto tempDeltaY = ri.data.mouse.lLastY;
            deltaX += tempDeltaX;
            deltaY += tempDeltaY;
            // Sapphire::Logger::GetInstance().Log("WM_INPUT(%d, %d) delta(%d, %d)\n", tempDeltaX, tempDeltaY, deltaX, deltaY);
        }
#else
#endif
        break;
    }
    }
}

void RawInput::Initialize()
{
    RAWINPUTDEVICE devices[2];

    devices[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
    devices[0].usUsage = HID_USAGE_GENERIC_MOUSE;
    devices[0].dwFlags = 0;
    devices[0].hwndTarget = 0;

    devices[1].usUsagePage = HID_USAGE_PAGE_GENERIC;
    devices[1].usUsage = HID_USAGE_GENERIC_KEYBOARD;
    devices[1].dwFlags = 0;
    devices[1].hwndTarget = 0;

    if (RegisterRawInputDevices(devices, 2, sizeof(RAWINPUTDEVICE)) == FALSE)
    {
        Sapphire::Logger::GetInstance().Log("ERROR: RawInput devices registration failes!\n");
    }
    else
    {
        Sapphire::Logger::GetInstance().Log("RawInput devices registration succeded.\n");
    }
}

void RawInput::PostFrame()
{
    deltaX = 0;
    deltaY = 0;
}

long RawInput::getMouseXDelta()
{
    return deltaX;
}

long RawInput::getMouseYDelta()
{
    return deltaY;
}

bool RawInput::isKeyDown(WPARAM virtualKeyCode)
{
    return virtualKeyState[virtualKeyCode];
}
