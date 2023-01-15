#include "GrayscalePass.h"
#include "RenderContext.h"
#include "../DX12Backend/DX12InputLayout.h"

Sapphire::GrayscalePass::GrayscalePass(RenderContext* renderContext, unsigned int width, unsigned int height)
{
	multiRenderTarget = new DX12MultiRenderTarget();
	multiRenderTarget->Add(renderContext->CreateRenderTarget(GrayscaleRT, width, height));
	depthBuffer = renderContext->CreateDepthBuffer(width, height);

	// Create Shaders
	vertexShader = new DX12Shader("fullscreen_vs.cso");
	pixelShader = new DX12Shader("grayscale_ps.cso");

	// Create Input Layout (has to be done before PSO, fix that)
	inputLayout = new DX12InputLayout();

	pipelineStates.PushBack(renderContext->CreatePipelineState(vertexShader, pixelShader, inputLayout));
	pipelineStates[0]->AddRenderTarget(multiRenderTarget->Get(0)->GetDxgiFormat());
	pipelineStates[0]->CreatePipelineState(renderContext->GetDevice(), vertexShader->GetBytecode(), pixelShader->GetBytecode(), inputLayout);
}

Sapphire::GrayscalePass::~GrayscalePass()
{
}

void Sapphire::GrayscalePass::PreRender(DX12CommandList* commandList)
{
}

void Sapphire::GrayscalePass::Render(DX12CommandList* commandList, RenderContext* renderContext, std::vector<GameObject*> objects)
{
	commandList->GetCommandList()->BeginEvent(1, "GrayscalePass", sizeof("GrayscalePass"));
	commandList->DrawEmpty();
	commandList->GetCommandList()->EndEvent();
}

void Sapphire::GrayscalePass::PostRender(DX12CommandList* commandList)
{
}
