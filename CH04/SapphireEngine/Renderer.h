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
		void CreateCommandAllocator();
		void CreateCommandList();
		void EnableDebugLayer();
		HWND hwnd;
		LONG width;
		LONG height;
		IDXGIFactory4* dxgiFactory;
		IDXGIAdapter1* dxgiAdapter;
		ID3D12Device* device;
		ID3D12CommandQueue* graphicsCommandQueue;
		ID3D12CommandAllocator* commandAllocator;
		ID3D12GraphicsCommandList* commandList;
	};
}
