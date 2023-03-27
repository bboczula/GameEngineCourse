#pragma once

#include "MessageQueueObserver.h"
#include "Logger.h"

class ImGuiHandler : public MessageQueueObserver
{
public:
	void Handle(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
	void Initialize() override;
	void PostFrame() override;
};