#pragma once

#include "DX12RenderTarget.h"
#include "DX12DepthBuffer.h"
#include "DX12Shader.h"
#include "DX12PipelineState.h"
#include "DX12Viewport.h"
#include "DeviceContext.h"
#include "RenderPass.h"
#include "Camera.h"

namespace Sapphire
{
	class RenderContext;
	class DX12InputLayout;
	class DX12ConstantBuffer;

	class ForwardRenderingPass : public Sapphire::RenderPass
	{
	public:
		ForwardRenderingPass(DeviceContext* deviceContext, RenderContext* renderContext);
		~ForwardRenderingPass();
		void Setup(DX12CommandList* commandList) override;
		void Render(DX12CommandList* commandList, RenderContext* renderContext, std::vector<GameObject*> objects) override;
		void Render(DX12CommandList* commandList, RenderContext* renderContext, std::vector<GameObject*> objects, DX12DepthBuffer* depthMap, Camera* shadowMapCamera);
		void Teardown(DX12CommandList* commandList) override;
		Sapphire::DX12RenderTarget* GetRenderTarget();
		void SetCamera(Camera* camera);
	private:
		DX12RenderTarget* renderTarget;
		DX12DepthBuffer* depthBuffer;
		DX12ConstantBuffer* constantBuffer;
		DX12Shader* pixelShader;
		DX12Shader* vertexShader;
		DX12PipelineState* dxPipelineState;
		DX12Viewport* viewport;
		DX12InputLayout* inputLayout;
		Camera* camera;
		float positionY = 1.0f;
	};
}