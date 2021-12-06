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
#include "DX12Device.h"
#include "DxgiManager.h"
#include "DX12CommandQueue.h"
#include "DX12CommandList.h"
#include "DX12DescriptorHeap.h"
#include "DX12RenderTarget.h"
#include "DX12PipelineState.h"
#include "DX12Resource.h"
#include "HardwareCapabilities.h"
#include "InputFile.h"

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxguid.lib")

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
		void RecordCommandList();
		void ExecuteCommandList();
		void PresentFrame();
		// CH09
		void CreateVertexBuffer(LONG width, LONG height);
		RendererSettings settings;
		DxgiManager* dxgiManager;
		DX12Device* device;
		DX12CommandQueue* commandQueue;
		DX12CommandList* commandList;
		DX12DescriptorHeap* rtvDescriptorHeap;
		DX12RenderTarget* renderTargets[FRAME_COUNT];
		DX12Shader* pixelShader;
		DX12Shader* vertexShader;
		DX12PipelineState* dxPipelineState;
		DX12Resource* dxResources[FRAME_COUNT];
		// CH09
		D3D12_VIEWPORT viewport;
		D3D12_RECT scissorRect;
		ID3D12Resource* vertexBuffer;
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	};
}
