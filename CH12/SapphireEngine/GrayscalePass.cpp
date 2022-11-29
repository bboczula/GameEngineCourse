#include "GrayscalePass.h"
#include "RenderContext.h"
#include "../DX12Backend/DX12InputLayout.h"

Sapphire::GrayscalePass::GrayscalePass(RenderContext* renderContext, unsigned int width, unsigned int height)
{
	renderTarget = renderContext->CreateRenderTarget(width, height);
	depthBuffer = renderContext->CreateDepthBuffer(width, height);

	// Create Shaders
	vertexShader = new DX12Shader("fullscreen_vs.cso");
	pixelShader = new DX12Shader("grayscale_ps.cso");

	// Create Input Layout (has to be done before PSO, fix that)
	inputLayout = new DX12InputLayout();

	pipelineStates.PushBack(renderContext->CreatePipelineState(vertexShader, pixelShader, inputLayout, true));
}

Sapphire::GrayscalePass::~GrayscalePass()
{
}

void Sapphire::GrayscalePass::PreRender(DX12CommandList* commandList)
{
}

void Sapphire::GrayscalePass::Render(DX12CommandList* commandList, RenderContext* renderContext, std::vector<GameObject*> objects)
{
	commandList->DrawEmpty();
}

void Sapphire::GrayscalePass::Teardown(DX12CommandList* commandList)
{
}
