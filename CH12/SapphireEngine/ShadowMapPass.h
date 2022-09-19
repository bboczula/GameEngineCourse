#pragma once

#include "DX12RenderTarget.h"
#include "DX12DepthBuffer.h"
#include "DX12Shader.h"
#include "DX12PipelineState.h"
#include "DX12Viewport.h"
#include "DeviceContext.h"
#include "RenderPass.h"
#include "OrthographicCamera.h"

namespace Sapphire
{
	class RenderContex;
	class DX12InputLayout;

	class ShadowMapPass : public Sapphire::RenderPass
	{
	public:
		ShadowMapPass(DeviceContext* deviceContext, RenderContext* renderContext);
		~ShadowMapPass();
		void Setup(DX12CommandList* commandList) override;
		void Render(DX12CommandList* commandList, RenderContext* renderContext, std::vector<GameObject*> objects) override;
		void Teardown(DX12CommandList* commandList) override;
		DX12DepthBuffer* GetDepthBuffer();
		OrthographicCamera* camera;
	private:
		DX12RenderTarget* renderTarget;
		DX12DepthBuffer* depthBuffer;
		DX12Shader* pixelShader;
		DX12Shader* vertexShader;
		DX12PipelineState* dxPipelineState;
		DX12Viewport* viewport;
		DX12InputLayout* inputLayout;
	};
}