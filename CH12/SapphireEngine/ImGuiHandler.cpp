#include "ImGuiHandler.h"

#include "EXTERNALS/imgui-master/backends/imgui_impl_win32.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void ImGuiHandler::Handle(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam);
}

void ImGuiHandler::Initialize()
{
}

void ImGuiHandler::PostFrame()
{
}
