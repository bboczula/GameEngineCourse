#pragma once

#include "../DX12Backend/DX12DescriptorHeap.h"
#include "../DX12Backend/DX12ConstantBuffer.h"
#include "../DX12Backend/DX12RenderTarget.h"
#include "../DX12Backend/DX12CommandList.h"
#include "DeviceContext.h"
#include "../DX12Backend/DX12Shader.h"
#include "../DX12Backend/DX12PipelineState.h"
#include "../DX12Backend/DX12Viewport.h"
#include "GameObject.h"
#include "Camera.h"
#include "ForwardRenderingPass.h"
#include "ShadowMapPass.h"
#include "GrayscalePass.h"

#include <vector>
#include <dxgi1_5.h>
#include <d3d12.h>
#include <iostream>
#include <wrl.h>
#include <windows.h>
#include <comdef.h>
#include <dxgidebug.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxguid.lib")

enum RenderTargetNames
{
	ForwardRenderingRT,
	Deferred_PositionRT,
	Deferred_AlbedoRT,
	Deferred_NormalRT,
	LightResolveRT,
	GrayscaleRT,
	ShadowMapRT,
	ShadowMapDepth,
	EnumSize
};

namespace Sapphire
{
	class DX12ConstantBuffer;
	class DX12VertexBuffer;
	class DX12IndexBuffer;
	class DX12PipelineState;
	class Light;
	class DeferredRenderingPass;
	class LightResolvePass;

	class RenderInterface
	{
	public:
		RenderInterface(HWND hwnd, unsigned int width, unsigned int height);
		~RenderInterface();
		void CreateResources(std::vector<GameObject*> objects);
		void CreateTextureResource(DX12Texture*& texture, UINT width, UINT height, PixelDefinition* source);
		DX12RenderTarget* CreateRenderTarget(RenderTargetNames name, UINT width, UINT height, DX12RenderTarget::Format format = DX12RenderTarget::Format::RGBA8_UNORM);
		DX12DepthBuffer* CreateDepthBuffer(UINT width, UINT height);
		DX12DepthBuffer* CreateDepthBufferWithSrv(RenderTargetNames name, UINT width, UINT height);
		DX12ConstantBuffer* CreateConstantBuffer();
		DX12PipelineState* CreatePipelineState(DX12Shader* vertexShader, DX12Shader* pixelShader, DX12InputLayout* inputLayout);
		D3D12_GPU_DESCRIPTOR_HANDLE GetSrvDescriptor(UINT32 index);
		D3D12_GPU_DESCRIPTOR_HANDLE GetSrvDescriptor(RenderTargetNames name);
		DX12DescriptorHeap* GetSrvDescriptorHeap();
		void Blit(DX12Resource* input);
		void Execute();
		DX12Device* GetDevice();
		DX12CommandList* GetCommandList();
		void SetSrvDescriptorHeap();
		HWND GetHwnd();
	private:
		void Blit(DX12Resource* source, DX12Resource* destination);
		DX12DescriptorHeap* rtvDescriptorHeap;
		DX12DescriptorHeap* dsvDescriptorHeap;
		DX12DescriptorHeap* srvDescriptorHeap;
		DX12CommandList* commandList;
		DX12Resource* uploadBuffer;
		DX12RenderTarget* renderTargets[FRAME_COUNT];
		DX12Resource* dxResources[FRAME_COUNT];
		DeviceContext* deviceContext;
		SIZE_T renderTargetList[RenderTargetNames::EnumSize];
		HWND hwnd;
	};
}

