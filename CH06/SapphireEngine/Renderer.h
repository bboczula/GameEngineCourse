#pragma once

#include <dxgi1_4.h>
#include <d3d12.h>
#include <iostream>
#include <wrl.h>
#include <windows.h>
#include <comdef.h>
#include <dxgidebug.h>

#include "Logger.h"
#include "Fence.h"
#include "Utils.h"
#include "CommandQueue.h"

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxguid.lib")

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
		//void CreateCommandQueue();
		void CreateSwapChain();
		void DisableDxgiMsgQueueMonitoring();
		void CreateCommandAllocator();
		void CreateCommandList();
		void CreateDescriptorHeap();
		void CreateFrameResources();
		void ResetCommandList();
		void RecordCommandList();
		void CloseCommandList();
		void ExecuteCommandList();
		void PresentFrame();
		//void WaitForPreviousFrame();
		void EnableDebugLayer();
		//Fence* fence;
		CommandQueue* commandQueue;
		HWND hwnd;
		LONG width;
		LONG height;
		UINT currentFrameIndex;
		ID3D12Device* device;
		//ID3D12CommandQueue* commandQueue;
		IDXGISwapChain3* dxgiSwapChain;
		IDXGIFactory4* dxgiFactory;
		IDXGIAdapter1* dxgiAdapter;
		ID3D12CommandAllocator* commandAllocator;
		ID3D12GraphicsCommandList* commandList;
		ID3D12DescriptorHeap* rtvHeap;
		UINT rtvDescriptorSize;
		ID3D12Resource* renderTargets[FRAME_COUNT];
	};
}