#pragma once

#include "RenderPass.h"

namespace Sapphire
{
	class RenderContext;
	class DX12Shader;
	class GameObject;

	class GrayscalePass : public RenderPass
	{
	public:
		GrayscalePass(RenderContext* renderContext, unsigned int width, unsigned int height);
		~GrayscalePass();
		void PreRender(DX12CommandList* commandList) override;
		void Render(DX12CommandList* commandList, RenderContext* renderContext, std::vector<GameObject*> objects) override;
		void PostRender(DX12CommandList* commandList) override;
	private:
		DX12Shader* pixelShader;
		DX12Shader* vertexShader;
	};
}