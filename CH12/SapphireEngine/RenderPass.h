#pragma once

#include "../DX12Backend/DX12CommandList.h"
#include "../DX12Backend/DX12RenderTarget.h"
#include "../DX12Backend/DX12PipelineState.h"
#include <vector>

namespace Sapphire
{
	// Including this was causing circular dependency and C2039 error
	class RenderContext;
	class GameObject;
	class DX12CommandList;
	class DX12RenderTarget;

	class RenderPass
	{
	public:
		virtual void PreRender(DX12CommandList* commandList) = 0;
		virtual void Render(DX12CommandList* commandList, RenderContext* renderContext, std::vector<GameObject*> objects) = 0;
		virtual void Teardown(DX12CommandList* commandList) = 0;
		void Setup(DX12CommandList* commandList)
		{
			const float clearColor[] = { 0.1176f, 0.1882f, 0.4470f, 1.0f };
			commandList->TransitionTo(renderTarget->GetResource(), D3D12_RESOURCE_STATE_RENDER_TARGET);
			commandList->SetRenderTarget(renderTarget, depthBuffer);
			commandList->ClearRenderTarget(renderTarget, clearColor);
			commandList->ClearDepthBuffer(depthBuffer);
			commandList->SetPipelineState(pipelineStates[0]);
		}
		DX12RenderTarget* GetRenderTarget()
		{
			return renderTarget;
		}
		DX12DepthBuffer* GetDepthBuffer()
		{
			return depthBuffer;
		}
		bool HasRenderTarget()
		{
			return renderTarget != nullptr;
		}
		bool HasDepthBuffer()
		{
			return depthBuffer != nullptr;
		}
	protected:
		DX12RenderTarget* renderTarget;
		DX12DepthBuffer* depthBuffer;
		std::vector<DX12PipelineState*> pipelineStates;
		DX12InputLayout* inputLayout;
	};
}