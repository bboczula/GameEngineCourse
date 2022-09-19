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
	long getMouseXDelta();
	long getMouseYDelta();
	bool isKeyDown(WPARAM virtualKeyCode);
private:
	long deltaX, deltaY;
	BOOL virtualKeyState[MAX_NUM_OF_KEYS];
	std::vector<BYTE> rawBuffer;
};