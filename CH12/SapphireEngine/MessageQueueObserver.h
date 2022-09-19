#pragma once

#include <Windows.h>

class MessageQueueObserver
{
public:
	virtual void Handle(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;
	virtual void Initialize() = 0;
	virtual void PostFrame() = 0;
	virtual ~MessageQueueObserver()
	{
	}
};