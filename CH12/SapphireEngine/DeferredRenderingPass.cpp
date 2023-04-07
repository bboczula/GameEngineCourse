#include "DeferredRenderingPass.h"

#define USE_PIX
#include "pix3.h"

#include "RenderInterface.h"
#include "../DX12Backend/DX12Shader.h"
#include "../DX12Backend/DX12InputLayout.h"
#include "../DX12Backend/DX12RenderTarget.h"
#include "../DX12Backend/DX12Texture.h"
#include "../DX12Backend/DX12RootSignature.h"

Sapphire::DeferredRenderingPass::DeferredRenderingPass(RenderInterface* renderInterface, unsigned int width, unsigned int height)
{
	multiRenderTarget = new DX12MultiRenderTarget();
	multiRenderTarget->Add(renderInterface->CreateRenderTarget(Deferred_PositionRT, width, height, DX12RenderTarget::Format::RGBA16_FLOAT));
	multiRenderTarget->Add(renderInterface->CreateRenderTarget(Deferred_NormalRT, width, height, DX12RenderTarget::Format::RGBA16_FLOAT));
	multiRenderTarget->Add(renderInterface->CreateRenderTarget(Deferred_AlbedoRT, width, height, DX12RenderTarget::Format::RGBA16_FLOAT));

	// Probably need the Depth Buffer too, although not optimized
	depthBuffer = renderInterface->CreateDepthBuffer(width, height);

	// Need Shaders (and add them to the compilation process)
	vertexShader = new DX12Shader("deferred_vs.cso");
	pixelShader = new DX12Shader("deferred_ps.cso");

	// Need Input Layout
	inputLayout = new DX12InputLayout();
	inputLayout->AppendElementT(VertexStream::Position, VertexStream::Normal, VertexStream::Tangent, VertexStream::TexCoord);

	// Need Root Signature
	rootSignature = new DX12RootSignature();
	rootSignature->AddParameter(DX12RootSignature::Type::Matrix); // 0 - ViewProjectionMatrix, b0
	rootSignature->AddParameter(DX12RootSignature::Type::Matrix); // 1 - WorldMatrix, b1
	rootSignature->AddParameter(DX12RootSignature::Type::Texture); // 2 - Color Texture, t0
	rootSignature->AddParameter(DX12RootSignature::Type::Texture); // 3 - Bump Texture, t1
	rootSignature->CreateRootSignature(renderInterface->GetDevice());

	// Need Pipeline State
	pipelineStates.PushBack(renderInterface->CreatePipelineState(vertexShader, pixelShader, inputLayout));
	pipelineStates[0]->AddRenderTarget(multiRenderTarget->Get(0)->GetDxgiFormat());
	pipelineStates[0]->AddRenderTarget(multiRenderTarget->Get(1)->GetDxgiFormat());
	pipelineStates[0]->AddRenderTarget(multiRenderTarget->Get(2)->GetDxgiFormat());
	pipelineStates[0]->CreatePipelineState(renderInterface->GetDevice(), vertexShader->GetBytecode(), pixelShader->GetBytecode(), inputLayout, rootSignature);
}

Sapphire::DeferredRenderingPass::~DeferredRenderingPass()
{
}

void Sapphire::DeferredRenderingPass::PreRender(DX12CommandList* commandList)
{
	commandList->SetConstantBuffer(0, 16, camera->GetViewProjectionMatrixPtr());
}

void Sapphire::DeferredRenderingPass::Render(DX12CommandList* commandList, RenderInterface* renderInterface, std::vector<GameObject*> objects)
{
	PIXBeginEvent(commandList->GetCommandList(), PIX_COLOR(255, 255, 255), "DeferredRenderingPass");
	for (int i = 0; i < objects.size(); i++)
	{
		if (!objects[i]->metaIsVisible.value)
		{
			continue;
		}
		if (objects[i]->numOfVertices != 0)
		{
			commandList->SetConstantBuffer(1, 16, &objects[i]->world);
			commandList->SetTexture(2, renderInterface->GetSrvDescriptor(objects[i]->texture->GetDescriptorIndex()));
			if (objects[i]->bumpMapWidth != 0)
			{
				commandList->SetTexture(3, renderInterface->GetSrvDescriptor(objects[i]->bumpMap->GetDescriptorIndex()));
			}
			// D3D12_GPU_DESCRIPTOR_HANDLE descriptor;
			// descriptor.ptr = srvDescriptorHeap->GetFirstGpuDescriptor().ptr + i * srvDescriptorHeap->GetDescriptorSize();
			// commandList->SetTexture(3, descriptor);
			//commandList->SetTexture(3, renderInterface->GetSrvDescriptor(objects[i]->texture->GetDescriptorIndex()));
			//commandList->Draw(objects[i]->geometry);
			//commandList->Draw(objects[i]->positionVertexBuffer, objects[i]->indexBuffer);
			//commandList->Draw(objects[i]->positionVertexBuffer, objects[i]->normalVertexBuffer, objects[i]->indexBuffer);
			// commandList->Draw(objects[i]->positionVertexBuffer, objects[i]->normalVertexBuffer, objects[i]->colorTexCoordVertexBuffer, objects[i]->indexBuffer);
			//commandList->Draw(objects[i]->positionVertexBuffer, objects[i]->normalVertexBuffer, objects[i]->indexBuffer);
			commandList->Draw(objects[i]->positionVertexBuffer, objects[i]->normalVertexBuffer, objects[i]->tangentVertexBuffer, objects[i]->colorTexCoordVertexBuffer, objects[i]->indexBuffer);
		}
	}
	PIXEndEvent(commandList->GetCommandList());
}

void Sapphire::DeferredRenderingPass::PostRender(DX12CommandList* commandList)
{
}

void Sapphire::DeferredRenderingPass::SetCamera(Camera* camera)
{
	this->camera = camera;
}
