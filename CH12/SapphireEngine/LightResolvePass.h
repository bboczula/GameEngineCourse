#pragma once

#include "RenderPass.h"

namespace Sapphire
{
	class LightResolvePass : public Sapphire::RenderPass
	{
	public:
		LightResolvePass(RenderContext* renderContext, unsigned int width, unsigned int height);
		~LightResolvePass();
		void PreRender(DX12CommandList* commandList) override;
		void Render(DX12CommandList* commandList, RenderContext* renderContext, std::vector<GameObject*> objects)  override;
		void PostRender(DX12CommandList* commandList)  override;
	private:
		DX12Shader* pixelShader;
		DX12Shader* vertexShader;
	};
}