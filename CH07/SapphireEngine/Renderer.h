#pragma once

#include <dxgi1_5.h>
#include <d3d12.h>
#include <iostream>
#include <wrl.h>
#include <windows.h>
#include <comdef.h>
#include <dxgidebug.h>
#include <d3dcompiler.h>
//  CH09
#include <DirectXMath.h>

#include "Logger.h"
#include "Utils.h"
#include "DX12CommandQueue.h"
#include "DX12CommandList.h"
#include "DX12DescriptorHeap.h"
#include "DX12RenderTarget.h"
#include "HardwareCapabilities.h"

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "D3DCompiler.lib")

#define FRAME_COUNT 2

namespace Sapphire
{

	struct Vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};

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
		void CreateCommandList();
		void CreateDescriptorHeap();
		void CreateRenderTargets();
		void RecordCommandList();
		void ExecuteCommandList();
		void PresentFrame();
		void EnableDebugLayer();
		bool IsVsyncDisabledAndTearingAllowed();
		RendererSettings settings;
		HardwareCapabilities hardwareCapabilities;
		DX12CommandQueue* commandQueue;
		DX12CommandList* commandList;
		DX12DescriptorHeap* rtvDescriptorHeap;
		DX12RenderTarget* renderTargets[FRAME_COUNT];
		HWND hwnd;
		LONG width;
		LONG height;
		UINT currentFrameIndex;
		ID3D12Device* device;
		IDXGISwapChain3* dxgiSwapChain;
		IDXGIFactory5* dxgiFactory;
		IDXGIAdapter1* dxgiAdapter;
		ID3D12Resource* resources[FRAME_COUNT];
	};
}
