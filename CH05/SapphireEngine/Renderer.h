#pragma once

#include <dxgi1_4.h>
#include <d3d12.h>
#include <iostream>
#include <wrl.h>
#include <windows.h>
#include <comdef.h>

#include "Logger.h"
#include "Utils.h"

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d12.lib")

#define FRAME_COUNT 2

namespace Sapphire
{
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
		void CreateDevice();
		void CreateCommandQueue();
		void CreateSwapChain();
		void CreateCommandAllocator();
		void CreateCommandList();
		void CreateSyncObjects();
		void ResetCommandList();
		void RecordCommandList();
		void CloseCommandList();
		void ExecuteCommandList();
		void PresentFrame();
		void WaitForPreviousFrame();
		void EnableDebugLayer();
		HWND hwnd;
		LONG width;
		LONG height;
		UINT currentFrameIndex;
		HANDLE fenceEvent;
		UINT64 fenceValue;
		ID3D12Fence* fence;
		ID3D12Device* device;
		ID3D12CommandQueue* commandQueue;
		IDXGISwapChain3* dxgiSwapChain;	// Needed "3" to get current buffer index
		IDXGIFactory4* dxgiFactory;
		IDXGIAdapter1* dxgiAdapter;
		ID3D12CommandAllocator* commandAllocator;
		ID3D12GraphicsCommandList* commandList;
	};
}
