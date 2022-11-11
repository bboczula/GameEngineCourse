#pragma once

#include "../DX12Backend/DX12RenderTarget.h"
#include "../DX12Backend/DX12ConstantBuffer.h"
#include "../DX12Backend/DX12DepthBuffer.h"
#include "../DX12Backend/DX12Shader.h"
#include "../DX12Backend/DX12PipelineState.h"
#include "../DX12Backend/DX12Viewport.h"
#include "RenderPass.h"

namespace Sapphire
{
	class RenderContext;
	class DX12InputLayout;
	class DX12ConstantBuffer;
	class Light;
	class Camera;

	class ForwardRenderingPass : public Sapphire::RenderPass
	{
	public:
		ForwardRenderingPass(RenderContext* renderContext, Light* light, unsigned int width, unsigned int height);
		~ForwardRenderingPass();
		void PreRender(DX12CommandList* commandList) override;
		void Render(DX12CommandList* commandList, RenderContext* renderContext, std::vector<GameObject*> objects) override;
		void Render(DX12CommandList* commandList, RenderContext* renderContext, std::vector<GameObject*> objects, DX12DepthBuffer* depthMap, Camera* shadowMapCamera);
		void Teardown(DX12CommandList* commandList) override;
		void SetCamera(Camera* camera);
	private:
		DX12ConstantBuffer* constantBuffer;
		DX12Shader* pixelShader;
		DX12Shader* vertexShader;
		DX12Shader* pixelShader_noBump;
		DX12Shader* vertexShader_noBump;
		Camera* camera;
		Light* light;
	};
}