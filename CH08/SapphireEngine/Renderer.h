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
#include "DxgiManager.h"
#include "DX12CommandQueue.h"
#include "DX12CommandList.h"
#include "DX12DescriptorHeap.h"
#include "DX12RenderTarget.h"
#include "DX12PipelineState.h"
#include "HardwareCapabilities.h"
#include "ShaderCompiler.h"

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxguid.lib")

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
		void CreateDxgiManager();
		void CreateDevice();
		void CreateCommandQueue();
		void CreateSwapChain();
		void CreateCommandList();
		void CreateDescriptorHeap();
		void CreateRenderTargets();
		void CreatePipelineState();
		void RecordCommandList();
		void ExecuteCommandList();
		void PresentFrame();
		void EnableDebugLayer();
		// CH09
		void CreateVertexBuffer();
		RendererSettings settings;
		HardwareCapabilities hardwareCapabilities;
		DxgiManager* dxgiManager;
		DX12CommandQueue* commandQueue;
		DX12CommandList* commandList;
		DX12DescriptorHeap* rtvDescriptorHeap;
		DX12RenderTarget* renderTargets[FRAME_COUNT];
		DX12Shader* pixelShader;
		DX12Shader* vertexShader;
		DX12PipelineState* dxPipelineState;
		HWND hwnd;
		LONG width;
		LONG height;
		ID3D12Device* device;
		ID3D12Resource* resources[FRAME_COUNT];
		// CH09
		D3D12_VIEWPORT viewport;
		D3D12_RECT scissorRect;
		ID3D12Resource* vertexBuffer;
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	};
}
