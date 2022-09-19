#pragma once

#include <Windows.h>
#include <list>
#include "Logger.h"
#include "Utils.h"
#include "MessageQueueObserver.h"

#define MAX_WINDOW_COMPONENTS 2

namespace Sapphire
{
	class WindowApplication
	{
	public:
		WindowApplication(UINT width, UINT height);
		~WindowApplication();
		void Run();
		void PostTick();
		void Attach(MessageQueueObserver* observer);
	protected:
		virtual void Initialize();
		virtual void Tick();
		void RegisterWindowClass();
		void CreateWindowInstance();
		static LRESULT CALLBACK ThisWindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		std::list<MessageQueueObserver*> observers;
		UINT16 numOfComponents;
		HWND hwnd;
		HINSTANCE instance;
		LONG width;
		LONG height;
	};
}