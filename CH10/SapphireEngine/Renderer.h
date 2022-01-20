#pragma once

#include <dxgi1_5.h>
#include <d3d12.h>
#include <iostream>
#include <wrl.h>
#include <windows.h>
#include <comdef.h>
#include <dxgidebug.h>
#include <vector>

#include "../SapphireMath/SapphireMath.h"

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
#include "DX12Geometry.h"
#include "HardwareCapabilities.h"
#include "InputFile.h"
#include "GameObject.h"

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxguid.lib")

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
		void Render(std::vector<GameObject*> objects);
		void CreateResources(std::vector<GameObject*> objects);
	private:
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
		DX12Viewport* viewport;
		D3D12_RECT scissorRect;
	};
}
