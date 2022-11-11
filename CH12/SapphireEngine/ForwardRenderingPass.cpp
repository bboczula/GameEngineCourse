#include "ForwardRenderingPass.h"
#include "RenderContext.h"
#include "../DX12Backend/DX12InputLayout.h"
#include "../DX12Backend/DX12ConstantBuffer.h"
#include "../DX12Backend/DX12Texture.h"
#include "Light.h"
#include "Camera.h"
#include "PerspectiveCamera.h"

Sapphire::ForwardRenderingPass::ForwardRenderingPass(RenderContext* renderContext, Light* light, unsigned int width, unsigned int height) : light(light)
{
	// Do I need Render Context here?
	// For example, I have to allocate descriptors somehow
	// Or maybe even I can ask RenderContext to create the Render Target for me
	// renderContext->CreateRenderTarget();
	// renderContext->CreateDepthBuffer();
	renderTarget = renderContext->CreateRenderTarget(width, height);
	renderTarget->GetResource()->GetResource()->SetName(L"ForwardRendering_RenderTarget");
	depthBuffer = renderContext->CreateDepthBuffer(width, height);

	// Create Constant Buffer for the light data
	constantBuffer = renderContext->CreateConstantBuffer();
	constantBuffer->UploadFloat4(light->GetPositionX(), light->GetPositionY(), light->GetPositionZ(), 0.0f);

	// Create Shaders
	vertexShader = new DX12Shader("directional_texture_vs.cso");
	pixelShader = new DX12Shader("directional_texture_ps.cso");
	vertexShader_noBump = new DX12Shader("directional_texture_vs_1.cso");
	pixelShader_noBump = new DX12Shader("directional_texture_ps_1.cso");

	// Create Input Layout
	inputLayout = new DX12InputLayout();
	inputLayout->AppendElement(VertexStream::Position);
	inputLayout->AppendElement(VertexStream::Normal);
	inputLayout->AppendElement(VertexStream::Tangent);
	inputLayout->AppendElement(VertexStream::TexCoord);

	// Create Pipeline State
	pipelineStates.push_back(renderContext->CreatePipelineState(vertexShader, pixelShader, inputLayout));
	pipelineStates.push_back(renderContext->CreatePipelineState(vertexShader_noBump, pixelShader_noBump, inputLayout));

	// Create Camera
	//camera = new Camera(1280.0f / 720.0f);
	//camera = new PerspectiveCamera(1280.0f / 720.0f);
}

Sapphire::ForwardRenderingPass::~ForwardRenderingPass()
{
	delete camera;
	delete pixelShader;
	delete vertexShader;
	delete pixelShader_noBump;
	delete vertexShader_noBump;
	delete depthBuffer;
	delete renderTarget;
}

void Sapphire::ForwardRenderingPass::PreRender(DX12CommandList* commandList)
{
	// camera->LogInfo();
	// Update the constant buffer
	constantBuffer->UploadFloat4(light->GetPositionX(), light->GetPositionY(), light->GetPositionZ(), 0.0f);

	const float clearColor[] = { 0.1176f, 0.1882f, 0.4470f, 1.0f };
	//unsigned int currentFrameIndex = deviceContext->GetCurrentFrameIndex();

	//commandList->Reset();
	// commandList->TransitionTo(renderTarget->GetResource(), D3D12_RESOURCE_STATE_RENDER_TARGET);
	// commandList->SetRenderTarget(renderTarget, depthBuffer);
	// commandList->ClearRenderTarget(renderTarget, clearColor);
	// Do I have to manage resource states?
	// commandList->ClearDepthBuffer(depthBuffer);
	commandList->SetConstantBuffer(0, 16, camera->GetViewProjectionMatrixPtr());
	//commandList->SetConstantBuffer(1, 16, camera->GetProjectionMatrixPtr());
}

void Sapphire::ForwardRenderingPass::Render(DX12CommandList* commandList, RenderContext* renderContext, std::vector<GameObject*> objects)
{
	// This could potentially be Render Pass
	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->numOfVertices != 0)
		{
			commandList->SetConstantBuffer(2, 16, &objects[i]->world);
			// D3D12_GPU_DESCRIPTOR_HANDLE descriptor;
			// descriptor.ptr = srvDescriptorHeap->GetFirstGpuDescriptor().ptr + i * srvDescriptorHeap->GetDescriptorSize();
			// commandList->SetTexture(3, descriptor);
			commandList->SetTexture(3, renderContext->GetSrvDescriptor(objects[i]->texture->GetDescriptorIndex()));
			//commandList->Draw(objects[i]->geometry);
			//commandList->Draw(objects[i]->positionVertexBuffer, objects[i]->indexBuffer);
			//commandList->Draw(objects[i]->positionVertexBuffer, objects[i]->normalVertexBuffer, objects[i]->indexBuffer);
			//commandList->Draw(objects[i]->positionVertexBuffer, objects[i]->normalVertexBuffer, objects[i]->colorTexCoordVertexBuffer, objects[i]->indexBuffer);
			commandList->Draw(objects[i]->positionVertexBuffer, objects[i]->normalVertexBuffer, objects[i]->tangentVertexBuffer, objects[i]->colorTexCoordVertexBuffer, objects[i]->indexBuffer);
		}
	}
}

void Sapphire::ForwardRenderingPass::Render(DX12CommandList* commandList, RenderContext* renderContext, std::vector<GameObject*> objects, DX12DepthBuffer* depthMap, Camera* shadowMapCamera)
{
	commandList->TransitionTo(depthMap->GetResource(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->numOfVertices != 0)
		{
			if (objects[i]->bumpMapWidth == 0)
			{
				commandList->SetPipelineState(pipelineStates[1]);
			}
			else
			{
				commandList->SetPipelineState(pipelineStates[0]);
			}
			commandList->SetConstantBuffer(1, 16, shadowMapCamera->GetViewProjectionMatrixPtr());
			commandList->SetConstantBuffer(2, 16, &objects[i]->world);
			// D3D12_GPU_DESCRIPTOR_HANDLE descriptor;
			// descriptor.ptr = srvDescriptorHeap->GetFirstGpuDescriptor().ptr + i * srvDescriptorHeap->GetDescriptorSize();
			// commandList->SetTexture(3, descriptor);
			commandList->SetTexture(3, renderContext->GetSrvDescriptor(objects[i]->texture->GetDescriptorIndex()));
			commandList->SetTexture(4, renderContext->GetSrvDescriptor(depthMap->GetDescriptorIndex()));
			if (objects[i]->bumpMapWidth != 0)
			{
				commandList->SetTexture(5, renderContext->GetSrvDescriptor(objects[i]->bumpMap->GetDescriptorIndex()));
			}
			commandList->SetConstantBuffer(6, renderContext->GetSrvDescriptor(constantBuffer->GetDescriptorIndex()));
			//commandList->Draw(objects[i]->geometry);
			//commandList->Draw(objects[i]->positionVertexBuffer, objects[i]->indexBuffer);
			//commandList->Draw(objects[i]->positionVertexBuffer, objects[i]->normalVertexBuffer, objects[i]->indexBuffer);
			//commandList->Draw(objects[i]->positionVertexBuffer, objects[i]->normalVertexBuffer, objects[i]->colorTexCoordVertexBuffer, objects[i]->indexBuffer);
			commandList->Draw(objects[i]->positionVertexBuffer, objects[i]->normalVertexBuffer, objects[i]->tangentVertexBuffer, objects[i]->colorTexCoordVertexBuffer, objects[i]->indexBuffer);
		}
	}
	commandList->TransitionTo(depthMap->GetResource(), D3D12_RESOURCE_STATE_DEPTH_WRITE);
}

void Sapphire::ForwardRenderingPass::Teardown(DX12CommandList* commandList)
{
	// THis is needed only for last Render Pass, maybe we should move it Render Context then?
	//commandList->TransitionTo(renderTarget->GetResource(), D3D12_RESOURCE_STATE_COPY_SOURCE);
	//commandList->Close();
}

//Sapphire::DX12RenderTarget* Sapphire::ForwardRenderingPass::GetRenderTarget()
//{
//	return renderTarget;
//}

void Sapphire::ForwardRenderingPass::SetCamera(Camera* camera)
{
	this->camera = camera;
}
