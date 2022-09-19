#pragma once

#include "DX12CommandList.h"

namespace Sapphire
{
	// Including this was causing circular dependency and C2039 error
	class RenderContext;
	class GameObject;

	class RenderPass
	{
	public:
		virtual void Setup(DX12CommandList* commandList) = 0;
		virtual void Render(DX12CommandList* commandList, RenderContext* renderContext, std::vector<GameObject*> objects) = 0;
		virtual void Teardown(DX12CommandList* commandList) = 0;
	};
}