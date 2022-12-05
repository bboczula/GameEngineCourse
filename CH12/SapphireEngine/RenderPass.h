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
		virtual void PostRender(DX12CommandList* commandList) = 0;
		void Setup(DX12CommandList* commandList)
		{
			// Barrier for all input resources
			for (unsigned int i = 0; i < inputResources.size(); i++)
			{
				commandList->TransitionTo(inputResources[i], D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
			}

			// Setup the mandatory resources
			const float clearColor[] = { 0.1176f, 0.1882f, 0.4470f, 1.0f };
			// Why do you really need this transition? Ah, ok, maybe because sometimes this resource might transition to other state in different passes
			// This is conditional though, should be denoted
			for (unsigned int i = 0; i < multiRenderTarget->Size(); i++)
			{
				commandList->TransitionTo(multiRenderTarget->Get(i)->GetResource(), D3D12_RESOURCE_STATE_RENDER_TARGET);
				commandList->ClearRenderTarget(multiRenderTarget->Get(i), clearColor);
			}
			commandList->SetRenderTarget(multiRenderTarget, depthBuffer);
			commandList->ClearDepthBuffer(depthBuffer);
			commandList->SetPipelineState(pipelineStates[0]);
		}
		DX12RenderTarget* GetRenderTarget(unsigned int index)
		{
			return multiRenderTarget->Get(index);
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
		void AddInputResource(DX12Resource* resource)
		{
			inputResources.push_back(resource);
		}
		DX12Resource* GetInputResource(unsigned int index)
		{
			return inputResources[index];
		}
	protected:
		using PipelineStateArray = TFixedArray<DX12PipelineState*, MAX_PIPELINE_STATE_NUM>;
		DX12DepthBuffer* depthBuffer;
		DX12InputLayout* inputLayout;
		DX12MultiRenderTarget* multiRenderTarget;
		PipelineStateArray pipelineStates;
		std::vector<DX12Resource*> inputResources;
	};
}