#include "PositionPass.h"

#include "RenderContext.h"
#include "../DX12Backend/DX12Shader.h"
#include "../DX12Backend/DX12InputLayout.h"
#include "../DX12Backend/DX12RenderTarget.h"

Sapphire::PositionPass::PositionPass(RenderContext* renderContext, unsigned int width, unsigned int height)
{
	multiRenderTarget = new DX12MultiRenderTarget();
	multiRenderTarget->Add(renderContext->CreateRenderTarget(Position_ColorRT, width, height, DX12RenderTarget::Format::RGBA16_FLOAT));
	multiRenderTarget->Add(renderContext->CreateRenderTarget(Position_NormalRT, width, height, DX12RenderTarget::Format::RGBA16_FLOAT));

	// Probably need the Depth Buffer too, although not optimized
	depthBuffer = renderContext->CreateDepthBuffer(width, height);

	// Need Shaders (and add them to the compilation process)
	vertexShader = new DX12Shader("position_vs.cso");
	pixelShader = new DX12Shader("position_ps.cso");

	// Need Input Layout
	inputLayout = new DX12InputLayout();
	inputLayout->AppendElementT(VertexStream::Position, VertexStream::Normal);

	// Need Pipeline State
	pipelineStates.PushBack(renderContext->CreatePipelineState(vertexShader, pixelShader, inputLayout));
}

void Sapphire::PositionPass::PreRender(DX12CommandList* commandList)
{
	commandList->SetConstantBuffer(0, 16, camera->GetViewProjectionMatrixPtr());
}

void Sapphire::PositionPass::Render(DX12CommandList* commandList, RenderContext* renderContext, std::vector<GameObject*> objects)
{
	commandList->GetCommandList()->BeginEvent(1, "PositionPass", sizeof("PositionPass"));
	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->numOfVertices != 0)
		{
			commandList->SetConstantBuffer(2, 16, &objects[i]->world);
			// D3D12_GPU_DESCRIPTOR_HANDLE descriptor;
			// descriptor.ptr = srvDescriptorHeap->GetFirstGpuDescriptor().ptr + i * srvDescriptorHeap->GetDescriptorSize();
			// commandList->SetTexture(3, descriptor);
			//commandList->SetTexture(3, renderContext->GetSrvDescriptor(objects[i]->texture->GetDescriptorIndex()));
			//commandList->Draw(objects[i]->geometry);
			//commandList->Draw(objects[i]->positionVertexBuffer, objects[i]->indexBuffer);
			//commandList->Draw(objects[i]->positionVertexBuffer, objects[i]->normalVertexBuffer, objects[i]->indexBuffer);
			//commandList->Draw(objects[i]->positionVertexBuffer, objects[i]->normalVertexBuffer, objects[i]->colorTexCoordVertexBuffer, objects[i]->indexBuffer);
			commandList->Draw(objects[i]->positionVertexBuffer, objects[i]->normalVertexBuffer, objects[i]->indexBuffer);
		}
	}
	commandList->GetCommandList()->EndEvent();
}

void Sapphire::PositionPass::PostRender(DX12CommandList* commandList)
{
}

void Sapphire::PositionPass::SetCamera(Camera* camera)
{
	this->camera = camera;
}
