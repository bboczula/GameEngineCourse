#include "GrayscalePass.h"
#include "RenderInterface.h"
#include "../DX12Backend/DX12InputLayout.h"
#include "../DX12Backend/DX12RootSignature.h"

#define USE_PIX
#include "pix3.h"

Sapphire::GrayscalePass::GrayscalePass(RenderInterface* renderInterface, unsigned int width, unsigned int height)
{
	multiRenderTarget = new DX12MultiRenderTarget();
	multiRenderTarget->Add(renderInterface->CreateRenderTarget(GrayscaleRT, width, height));
	depthBuffer = renderInterface->CreateDepthBuffer(width, height);

	// Create Shaders
	vertexShader = new DX12Shader("fullscreen_vs.cso");
	pixelShader = new DX12Shader("grayscale_ps.cso");

	// Create Input Layout (has to be done before PSO, fix that)
	inputLayout = new DX12InputLayout();

	// Need Root Signature, even if empty
	rootSignature = new DX12RootSignature();
	rootSignature->CreateRootSignature(renderInterface->GetDevice());

	pipelineStates.PushBack(renderInterface->CreatePipelineState(vertexShader, pixelShader, inputLayout));
	pipelineStates[0]->AddRenderTarget(multiRenderTarget->Get(0)->GetDxgiFormat());
	pipelineStates[0]->CreatePipelineState(renderInterface->GetDevice(), vertexShader->GetBytecode(), pixelShader->GetBytecode(), inputLayout, rootSignature);
}

Sapphire::GrayscalePass::~GrayscalePass()
{
}

void Sapphire::GrayscalePass::PreRender(DX12CommandList* commandList)
{
}

void Sapphire::GrayscalePass::Render(DX12CommandList* commandList, RenderInterface* renderInterface, std::vector<GameObject*> objects, std::vector<LightObject*> lights)
{
	PIXBeginEvent(commandList->GetCommandList(), PIX_COLOR(255, 255, 255), "GrayscalePass");
	commandList->DrawEmpty();
	PIXEndEvent(commandList->GetCommandList());
}

void Sapphire::GrayscalePass::PostRender(DX12CommandList* commandList)
{
}
