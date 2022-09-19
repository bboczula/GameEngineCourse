#pragma once

#include "DX12DescriptorHeap.h"
#include "DX12RenderTarget.h"
#include "DX12CommandList.h"
#include "DeviceContext.h"
#include "DX12Shader.h"
#include "DX12PipelineState.h"
#include "DX12Viewport.h"
#include "GameObject.h"
#include "Camera.h"
#include "ForwardRenderingPass.h"
#include "ShadowMapPass.h"

#include <vector>

namespace Sapphire
{
	//class ForwardRenderingPass;
	//class ShadowMapPass;
	class BlitPass;
	class DX12ConstantBuffer;

	class RenderContext
	{
	public:
		RenderContext(DeviceContext* deviceContext);
		~RenderContext();
		void Setup();
		void CreateResources(DeviceContext* deviceContext, std::vector<GameObject*> objects);
		void CreateTextureResource(DeviceContext* deviceContext, DX12Texture*& texture, UINT width, UINT height, PixelDefinition* source);
		DX12RenderTarget* CreateRenderTarget(DeviceContext* deviceContext, UINT width, UINT height);
		DX12DepthBuffer* CreateDepthBuffer(DeviceContext* deviceContext, UINT width, UINT height);
		DX12DepthBuffer* CreateDepthBufferWithSrv(DeviceContext* deviceContext, UINT width, UINT height);
		DX12ConstantBuffer* CreateConstantBuffer();
		D3D12_GPU_DESCRIPTOR_HANDLE GetSrvDescriptor(UINT32 index);
		void Render(std::vector<GameObject*> objects);
		void Teardown();
		void Execute(DeviceContext* deviceContext);
		DX12RenderTarget* GetRenderTarget();
		void SetCamera(Camera* camera);
	private:
		DX12DescriptorHeap* rtvDescriptorHeap;
		DX12DescriptorHeap* dsvDescriptorHeap;
		DX12DescriptorHeap* srvDescriptorHeap;
		//DX12RenderTarget* renderTarget;
		//DX12DepthBuffer* depthBuffer;
		DX12CommandList* commandList;
		//DX12Shader* pixelShader;
		//DX12Shader* vertexShader;
		//DX12PipelineState* dxPipelineState;
		//DX12Viewport* viewport;
		DX12Resource* uploadBuffer;
		DX12RenderTarget* renderTargets[FRAME_COUNT];
		DX12Resource* dxResources[FRAME_COUNT];
		//Camera* camera;
		ForwardRenderingPass* renderPass;
		ShadowMapPass* shadowMapPass;
		BlitPass* blitPass;
		DeviceContext* deviceContext;
	};
}

