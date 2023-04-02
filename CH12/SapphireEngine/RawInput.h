#pragma once

#include "MessageQueueObserver.h"
#include "Logger.h"
#include <Windows.h>
#include <windowsx.h>
#include <iostream>
#include <hidusage.h>
#include <vector>

#define MAX_NUM_OF_KEYS 254

class RawInput : public MessageQueueObserver
{
public:
	void Handle(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
	void Initialize() override;
	void PostFrame() override;
	long GetMouseXDelta();
	long GetMouseYDelta();
	bool IsKeyDown(WPARAM virtualKeyCode);
	bool WasKeyDown(WPARAM virtualKeyCode);
private:
	void HandleKeyboardInput(const RAWINPUT& rawInput);
	void HandleMouseInput(const RAWINPUT& rawInput);
	BOOL virtualKeyState[MAX_NUM_OF_KEYS];
	BOOL prevVirtualKeyState[MAX_NUM_OF_KEYS];
	std::pair<LONG, LONG> mouseDelta;
	std::vector<BYTE> rawBuffer;
};