#include "RawInput.h"

void RawInput::Handle(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_INPUT:
    {
        UINT dwSize = { 0 };
        GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));

        rawBuffer.resize(dwSize);
        GetRawInputData((HRAWINPUT)lParam, RID_INPUT, rawBuffer.data(), &dwSize, sizeof(RAWINPUTHEADER));

        auto& rawInput = reinterpret_cast<const RAWINPUT&>(*rawBuffer.data());
        if (rawInput.header.dwType == RIM_TYPEKEYBOARD)
        {
              HandleKeyboardInput(rawInput);
        }
        else if (rawInput.header.dwType == RIM_TYPEMOUSE)
        {
              HandleMouseInput(rawInput);
        }
    }
    }
}

void RawInput::HandleKeyboardInput(const RAWINPUT& rawInput)
{
      prevVirtualKeyState[rawInput.data.keyboard.VKey] = virtualKeyState[rawInput.data.keyboard.VKey];
      if (rawInput.data.keyboard.Flags == 0)
      {
            virtualKeyState[rawInput.data.keyboard.VKey] = true;
      }
      else
      {
            virtualKeyState[rawInput.data.keyboard.VKey] = false;
      }
}

void RawInput::HandleMouseInput(const RAWINPUT& rawInput)
{
      mouseDelta.first += rawInput.data.mouse.lLastX;
      mouseDelta.second += rawInput.data.mouse.lLastY;
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
    mouseDelta.first = 0;
    mouseDelta.second = 0;
}

long RawInput::getMouseXDelta()
{
    return mouseDelta.first;
}

long RawInput::getMouseYDelta()
{
    return mouseDelta.second;
}

bool RawInput::isKeyDown(WPARAM virtualKeyCode)
{
    return virtualKeyState[virtualKeyCode];
}

bool RawInput::wasKeyDown(WPARAM virtualKeyCode)
{
    return prevVirtualKeyState[virtualKeyCode] and !virtualKeyState[virtualKeyCode];
}
