#pragma once

#include "../DX12Backend/DX12CommandList.h"
#include "../DX12Backend/DX12RenderTarget.h"
#include "../DX12Backend/DX12MultiRenderTarget.h"
#include "../DX12Backend/DX12PipelineState.h"
#include <vector>

#include "FixedArray.h"

#define MAX_PIPELINE_STATE_NUM 2U

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
			commandList->TransitionTo(multiRenderTarget->Get(0)->GetResource(), D3D12_RESOURCE_STATE_RENDER_TARGET);
			commandList->SetRenderTarget(multiRenderTarget, depthBuffer);
			commandList->ClearRenderTarget(multiRenderTarget->Get(0), clearColor);
			commandList->ClearDepthBuffer(depthBuffer);
			commandList->SetPipelineState(pipelineStates[0]);
		}
		DX12RenderTarget* GetRenderTarget()
		{
			return multiRenderTarget->Get(0);
		}
		DX12DepthBuffer* GetDepthBuffer()
		{
			return depthBuffer;
		}
		bool HasRenderTarget()
		{
			return multiRenderTarget->Get(0) != nullptr;
		}
		bool HasDepthBuffer()
		{
			return depthBuffer != nullptr;
		}
	protected:
		using PipelineStateArray = TFixedArray<DX12PipelineState*, MAX_PIPELINE_STATE_NUM>;
		DX12DepthBuffer* depthBuffer;
		DX12InputLayout* inputLayout;
		DX12MultiRenderTarget* multiRenderTarget;
		PipelineStateArray pipelineStates;
	};
}