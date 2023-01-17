#include "DeferredRenderingPass.h"

#include "RenderContext.h"
#include "../DX12Backend/DX12Shader.h"
#include "../DX12Backend/DX12InputLayout.h"
#include "../DX12Backend/DX12RenderTarget.h"
#include "../DX12Backend/DX12Texture.h"

Sapphire::DeferredRenderingPass::DeferredRenderingPass(RenderContext* renderContext, unsigned int width, unsigned int height)
{
	multiRenderTarget = new DX12MultiRenderTarget();
	multiRenderTarget->Add(renderContext->CreateRenderTarget(Deferred_PositionRT, width, height, DX12RenderTarget::Format::RGBA16_FLOAT));
	multiRenderTarget->Add(renderContext->CreateRenderTarget(Deferred_NormalRT, width, height, DX12RenderTarget::Format::RGBA16_FLOAT));
	multiRenderTarget->Add(renderContext->CreateRenderTarget(Deferred_AlbedoRT, width, height, DX12RenderTarget::Format::RGBA16_FLOAT));

	// Probably need the Depth Buffer too, although not optimized
	depthBuffer = renderContext->CreateDepthBuffer(width, height);

	// Need Shaders (and add them to the compilation process)
	vertexShader = new DX12Shader("deferred_vs.cso");
	pixelShader = new DX12Shader("deferred_ps.cso");

	// Need Input Layout
	inputLayout = new DX12InputLayout();
	inputLayout->AppendElementT(VertexStream::Position, VertexStream::Normal, VertexStream::TexCoord);

	// Need Pipeline State
	pipelineStates.PushBack(renderContext->CreatePipelineState(vertexShader, pixelShader, inputLayout));
	pipelineStates[0]->AddRenderTarget(multiRenderTarget->Get(0)->GetDxgiFormat());
	pipelineStates[0]->AddRenderTarget(multiRenderTarget->Get(1)->GetDxgiFormat());
	pipelineStates[0]->AddRenderTarget(multiRenderTarget->Get(2)->GetDxgiFormat());
	pipelineStates[0]->CreatePipelineState(renderContext->GetDevice(), vertexShader->GetBytecode(), pixelShader->GetBytecode(), inputLayout);
}

void Sapphire::DeferredRenderingPass::PreRender(DX12CommandList* commandList)
{
	commandList->SetConstantBuffer(0, 16, camera->GetViewProjectionMatrixPtr());
}

void Sapphire::DeferredRenderingPass::Render(DX12CommandList* commandList, RenderContext* renderContext, std::vector<GameObject*> objects)
{
	commandList->GetCommandList()->BeginEvent(1, "DeferredRenderingPass", sizeof("DeferredRenderingPass"));
	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->numOfVertices != 0)
		{
			commandList->SetConstantBuffer(2, 16, &objects[i]->world);
			commandList->SetTexture(3, renderContext->GetSrvDescriptor(objects[i]->texture->GetDescriptorIndex()));
			// D3D12_GPU_DESCRIPTOR_HANDLE descriptor;
			// descriptor.ptr = srvDescriptorHeap->GetFirstGpuDescriptor().ptr + i * srvDescriptorHeap->GetDescriptorSize();
			// commandList->SetTexture(3, descriptor);
			//commandList->SetTexture(3, renderContext->GetSrvDescriptor(objects[i]->texture->GetDescriptorIndex()));
			//commandList->Draw(objects[i]->geometry);
			//commandList->Draw(objects[i]->positionVertexBuffer, objects[i]->indexBuffer);
			//commandList->Draw(objects[i]->positionVertexBuffer, objects[i]->normalVertexBuffer, objects[i]->indexBuffer);
			commandList->Draw(objects[i]->positionVertexBuffer, objects[i]->normalVertexBuffer, objects[i]->colorTexCoordVertexBuffer, objects[i]->indexBuffer);
			//commandList->Draw(objects[i]->positionVertexBuffer, objects[i]->normalVertexBuffer, objects[i]->indexBuffer);
		}
	}
	commandList->GetCommandList()->EndEvent();
}

void Sapphire::DeferredRenderingPass::PostRender(DX12CommandList* commandList)
{
}

void Sapphire::DeferredRenderingPass::SetCamera(Camera* camera)
{
	this->camera = camera;
}
