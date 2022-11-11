#pragma once

#include "../DX12Backend/DX12DepthBuffer.h"
#include "../DX12Backend/DX12Shader.h"
#include "../DX12Backend/DX12PipelineState.h"
#include "../DX12Backend/DX12Viewport.h"
#include "RenderPass.h"
#include "OrthographicCamera.h"

namespace Sapphire
{
	class RenderContex;
	class DX12InputLayout;
	class Light;
	class Arcball;

	class ShadowMapPass : public Sapphire::RenderPass
	{
	public:
		ShadowMapPass(RenderContext* renderContext, Light* light);
		~ShadowMapPass();
		void PreRender(DX12CommandList* commandList) override;
		void Render(DX12CommandList* commandList, RenderContext* renderContext, std::vector<GameObject*> objects) override;
		void Teardown(DX12CommandList* commandList) override;
		DX12DepthBuffer* GetDepthBuffer();
		OrthographicCamera* camera;
		Arcball* arcball;
	private:
		DX12Shader* pixelShader;
		DX12Shader* vertexShader;
		Light* light;
	};
}