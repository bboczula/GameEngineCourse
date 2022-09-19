#pragma once

#include "RenderPass.h"

namespace Sapphire
{
	class BlitPass : public RenderPass
	{
	public:
		BlitPass();
		~BlitPass();
		void Setup(DX12CommandList* commandList) override;
		void Render(DX12CommandList* commandList, RenderContext* renderContext, std::vector<GameObject*> objects) override;
		void Render(DX12CommandList* commandList, RenderContext* renderContext, DX12Resource* source, DX12Resource* destination);
		void Teardown(DX12CommandList* commandList) override;
	private:

	};
}