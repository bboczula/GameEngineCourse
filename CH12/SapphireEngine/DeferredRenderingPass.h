#pragma once

#include "RenderPass.h"

namespace Sapphire
{
	class RenderContex;
	class Camera;

	class DeferredRenderingPass : public RenderPass
	{
	public:
		// You manually have to create Constructors
		DeferredRenderingPass(RenderContext* renderContext, unsigned int width, unsigned int height);
		~DeferredRenderingPass();
		void PreRender(DX12CommandList* commandList) override;
		void Render(DX12CommandList* commandList, RenderContext* renderContext, std::vector<GameObject*> objects) override;
		void PostRender(DX12CommandList* commandList) override;
		void SetCamera(Camera* camera);
	private:
		DX12Shader* pixelShader;
		DX12Shader* vertexShader;
		Camera* camera;
	};
}