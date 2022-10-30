#pragma once

#include "../DX12Backend/DX12CommandList.h"
#include <vector>

namespace Sapphire
{
	// Including this was causing circular dependency and C2039 error
	class RenderContext;
	class GameObject;
	class DX12CommandList;

	class RenderPass
	{
	public:
		virtual void Setup(DX12CommandList* commandList) = 0;
		virtual void Render(DX12CommandList* commandList, RenderContext* renderContext, std::vector<GameObject*> objects) = 0;
		virtual void Teardown(DX12CommandList* commandList) = 0;
	};
}