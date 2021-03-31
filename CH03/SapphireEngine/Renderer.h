#pragma once

#include <dxgi1_4.h>
#include <iostream>
#include <wrl.h>
#include <windows.h>
#include <comdef.h>

#include "Logger.h"
#include "Utils.h"

#pragma comment(lib, "dxgi.lib")

namespace Sapphire
{
	class Renderer
	{
	public:
		Renderer();
		~Renderer();
	private:
		void CreateDxgiFactory();
		void EnumerateAdapters();
		void EnumerateOutputs(IDXGIAdapter1* currentAdapter);
		void LogOutputInfo(IDXGIOutput* output);
		void LogAdapterInfo(IDXGIAdapter1* adapter);
		IDXGIFactory4* dxgiFactory;
		IDXGIAdapter1* dxgiAdapter;
	};
}
