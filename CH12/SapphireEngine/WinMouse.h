#pragma once

#include <Windows.h>
#include <windowsx.h>

#include "Logger.h"
#include "MessageQueueObserver.h"

class WinMouse : public MessageQueueObserver
{
public:
	WinMouse();
	~WinMouse();
	void Handle(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
	void OnMouseMove(const HWND& hwnd, const WPARAM& wParam, LPARAM& lParam);
	void OnSetCursor(LPARAM& lParam);
	void Initialize() override;
	void PostFrame() override;
	int getMousePositionX();
	int getMousePositionY();
private:
	short mousePosX, mousePosY;
	bool isTrackMouseEventRequired = true;
	POINTS lastPoint;
};