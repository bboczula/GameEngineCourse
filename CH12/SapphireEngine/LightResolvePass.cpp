
#include "LightResolvePass.h"

Sapphire::LightResolvePass::LightResolvePass()
{
	// You have to create at leas empty Multi Render Target
	multiRenderTarget = new DX12MultiRenderTarget();
}

Sapphire::LightResolvePass::~LightResolvePass()
{
}

void Sapphire::LightResolvePass::PreRender(DX12CommandList* commandList)
{
}

void Sapphire::LightResolvePass::Render(DX12CommandList* commandList, RenderContext* renderContext, std::vector<GameObject*> objects)
{
	commandList->GetCommandList()->BeginEvent(1, "LightResolvePass", sizeof("ShadowMapPass"));
	commandList->GetCommandList()->EndEvent();
}

void Sapphire::LightResolvePass::PostRender(DX12CommandList* commandList)
{
}
