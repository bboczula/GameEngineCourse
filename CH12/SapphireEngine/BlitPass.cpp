#include "BlitPass.h"

Sapphire::BlitPass::BlitPass()
{
}

Sapphire::BlitPass::~BlitPass()
{
}

void Sapphire::BlitPass::Setup(DX12CommandList* commandList)
{
	// How about shaders, PSO, etc.? Should I just leave it alone?
	// Or maybe I should set some empty ones?
}

void Sapphire::BlitPass::Render(DX12CommandList* commandList, RenderContext* renderContext, std::vector<GameObject*> objects)
{
}

void Sapphire::BlitPass::Render(DX12CommandList* commandList, RenderContext* renderContext, DX12Resource* source, DX12Resource* destination)
{
	commandList->TransitionTo(source, D3D12_RESOURCE_STATE_COPY_SOURCE);
	commandList->TransitionTo(destination, D3D12_RESOURCE_STATE_COPY_DEST);
	commandList->CopyTextureRegion(source, destination);
	commandList->TransitionTo(destination, D3D12_RESOURCE_STATE_PRESENT);
}

void Sapphire::BlitPass::Teardown(DX12CommandList* commandList)
{
}
