#pragma once

#include "Utils.h"
#include "Logger.h"
#include "HardwareCapabilities.h"
#include "DX12CommandQueue.h"

#include <dxgi1_5.h>

#pragma comment(lib, "dxgi.lib")

namespace Sapphire
{
	class DxgiManager
	{
		friend class Renderer;
	public:
		DxgiManager();
		~DxgiManager();
		void CreateSwapChain(DX12CommandQueue* commandQueue, HWND hwnd, bool vsyncEnabled);
		void PresentFrame(bool vsyncEnabled);
		void DisableDxgiMsgQueueMonitoring(HWND hwnd);
	private:
		void CreateDxgiFactory();
		void GetCapabilities();
		void EnumAdapters();
		void LogAdapterInfo(IDXGIAdapter1* adapter);
		void EnumerateOutputs(IDXGIAdapter1* currentAdapter);
		void LogOutputInfo(IDXGIOutput* output);
		UINT currentFrameIndex;
		HardwareCapabilities hardwareCapabilities;
		IDXGIFactory5* dxgiFactory;
		IDXGIAdapter1* dxgiAdapter;
		IDXGISwapChain3* dxgiSwapChain;
	};
}