#pragma once

#include <dxgi1_5.h>
#include <d3d12.h>
#include <iostream>
#include <wrl.h>
#include <windows.h>
#include <comdef.h>
#include <dxgidebug.h>

#include "Logger.h"
#include "Utils.h"
#include "CommandQueue.h"
#include "HardwareCapabilities.h"

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxguid.lib")

#define FRAME_COUNT 2

namespace Sapphire
{
	struct RendererSettings
	{
		bool isVsyncEnabled = true;
	};

	class Renderer
	{
	public:
		Renderer(HWND hwnd, LONG width, LONG height);
		~Renderer();
		void Render();
	private:
		void CreateDxgiFactory();
		void EnumerateAdapters();
		void EnumerateOutputs(IDXGIAdapter1* currentAdapter);
		void LogOutputInfo(IDXGIOutput* output);
		void LogAdapterInfo(IDXGIAdapter1* adapter);
		void GetCapabilites();
		void CreateDevice();
		void CreateCommandQueue();
		void CreateSwapChain();
		void DisableDxgiMsgQueueMonitoring();
		void CreateCommandAllocator();
		void CreateCommandList();
		//void CreateDescriptorHeap();
		//void CreateFrameResources();
		void ResetCommandList();
		void RecordCommandList();
		void CloseCommandList();
		void ExecuteCommandList();
		void PresentFrame();
		void EnableDebugLayer();
		bool IsVsyncDisabledAndTearingAllowed();
		RendererSettings settings;
		HardwareCapabilities hardwareCapabilities;
		CommandQueue* commandQueue;
		HWND hwnd;
		LONG width;
		LONG height;
		UINT currentFrameIndex;
		ID3D12Device* device;
		IDXGISwapChain3* dxgiSwapChain;
		IDXGIFactory5* dxgiFactory;
		IDXGIAdapter1* dxgiAdapter;
		ID3D12CommandAllocator* commandAllocator;
		ID3D12GraphicsCommandList* commandList;
		ID3D12DescriptorHeap* rtvHeap;
		UINT rtvDescriptorSize;
		ID3D12Resource* renderTargets[FRAME_COUNT];
	};
}
