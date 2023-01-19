#pragma once

#include "RenderPass.h"

namespace Sapphire
{
	class LightResolvePass : public Sapphire::RenderPass
	{
	public:
		LightResolvePass();
		~LightResolvePass();
		void PreRender(DX12CommandList* commandList) override;
		void Render(DX12CommandList* commandList, RenderContext* renderContext, std::vector<GameObject*> objects)  override;
		void PostRender(DX12CommandList* commandList)  override;
	};
}