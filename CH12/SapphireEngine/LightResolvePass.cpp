
#include "LightResolvePass.h"
#include "RenderInterface.h"
#include "../DX12Backend/DX12InputLayout.h"
#include "../DX12Backend/DX12RenderTarget.h"
#include "../DX12Backend/DX12RootSignature.h"

#define USE_PIX
#include "pix3.h"

Sapphire::LightResolvePass::LightResolvePass(RenderInterface* renderInterface, unsigned int width, unsigned int height)
{
	// You have to create at leas empty Multi Render Target
	// Can't move this to the constructor, because  derived classes needs thin in their constructor, would have
	// to create RenderPass constructor.
	multiRenderTarget = new DX12MultiRenderTarget();
	multiRenderTarget->Add(renderInterface->CreateRenderTarget(LightResolveRT, width, height));

	// It is required to create depth buffer, even if you're not using it
	depthBuffer = renderInterface->CreateDepthBuffer(width, height);

	// Shaders are required if you have PSO
	vertexShader = new DX12Shader("bypass_vs.cso");
	pixelShader = new DX12Shader("bypass_ps.cso");

	// Input Layout required for PSO
	inputLayout = new DX12InputLayout();
	inputLayout->AppendElementT(VertexStream::Position, VertexStream::Normal, VertexStream::TexCoord);

	// Need Root Signature
	rootSignature = new DX12RootSignature();
	rootSignature->AddParameter(DX12RootSignature::Type::Matrix); // Not Needed
	rootSignature->AddParameter(DX12RootSignature::Type::Matrix); // Not Needed
	rootSignature->AddParameter(DX12RootSignature::Type::Matrix); // Not Needed
	rootSignature->AddParameter(DX12RootSignature::Type::Texture); // Not Needed
	rootSignature->AddParameter(DX12RootSignature::Type::Texture); // Not Needed
	rootSignature->AddParameter(DX12RootSignature::Type::Texture); // Not Needed
	rootSignature->AddParameter(DX12RootSignature::Type::ConstantBuffer); // Not Needed
	rootSignature->CreateRootSignature(renderInterface->GetDevice());

	// Once you have Render Target, PSO is required
	// Create Pipeline State
	pipelineStates.PushBack(renderInterface->CreatePipelineState(vertexShader, pixelShader, inputLayout));
	pipelineStates[0]->AddRenderTarget(multiRenderTarget->Get(0)->GetDxgiFormat());
	pipelineStates[0]->CreatePipelineState(renderInterface->GetDevice(), vertexShader->GetBytecode(), pixelShader->GetBytecode(), inputLayout, rootSignature);
}

Sapphire::LightResolvePass::~LightResolvePass()
{
}

void Sapphire::LightResolvePass::PreRender(DX12CommandList* commandList)
{
}

void Sapphire::LightResolvePass::Render(DX12CommandList* commandList, RenderInterface* renderInterface, std::vector<GameObject*> objects, std::vector<LightObject*> lights)
{
	PIXBeginEvent(commandList->GetCommandList(), PIX_COLOR(255, 255, 255), "LightResolvePass");
	PIXEndEvent(commandList->GetCommandList());
}

void Sapphire::LightResolvePass::PostRender(DX12CommandList* commandList)
{
}
