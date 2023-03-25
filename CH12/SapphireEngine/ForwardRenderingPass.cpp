#include "ForwardRenderingPass.h"
#include "RenderInterface.h"
#include "../DX12Backend/DX12InputLayout.h"
#include "../DX12Backend/DX12ConstantBuffer.h"
#include "../DX12Backend/DX12Texture.h"
#include "../DX12Backend/DX12RootSignature.h"
#include "Light.h"
#include "Camera.h"
#include "PerspectiveCamera.h"

#define USE_PIX
#include "pix3.h"

Sapphire::ForwardRenderingPass::ForwardRenderingPass(RenderInterface* renderInterface, Light* light, unsigned int width, unsigned int height) : light(light)
{
	multiRenderTarget = new DX12MultiRenderTarget();
	multiRenderTarget->Add(renderInterface->CreateRenderTarget(ForwardRenderingRT, width, height));
	depthBuffer = renderInterface->CreateDepthBuffer(width, height);

	// Create Constant Buffer for the light data
	constantBuffer = renderInterface->CreateConstantBuffer();
	constantBuffer->UploadFloat4(light->GetPositionX(), light->GetPositionY(), light->GetPositionZ(), 0.0f);

	// Create Shaders
	vertexShader = new DX12Shader("directional_texture_vs.cso");
	pixelShader = new DX12Shader("directional_texture_ps.cso");
	vertexShader_noBump = new DX12Shader("directional_texture_vs_1.cso");
	pixelShader_noBump = new DX12Shader("directional_texture_ps_1.cso");

	// Create Input Layout
	inputLayout = new DX12InputLayout();
	inputLayout->AppendElementT(VertexStream::Position, VertexStream::Normal, VertexStream::Tangent, VertexStream::TexCoord);

	// Need Root Signature
	rootSignature = new DX12RootSignature();
	rootSignature->AddParameter(DX12RootSignature::Type::Matrix); // 0 - ViewProjectionMatrix, b0
	rootSignature->AddParameter(DX12RootSignature::Type::Matrix); // 1 - viewProjectionMatrix, b1
	rootSignature->AddParameter(DX12RootSignature::Type::Matrix); // 2 - WorldMatrix, b2
	rootSignature->AddParameter(DX12RootSignature::Type::Texture); // 3 - Color Texture, t0
	rootSignature->AddParameter(DX12RootSignature::Type::Texture); // 4 - Shadow Map Depth, t1
	rootSignature->AddParameter(DX12RootSignature::Type::Texture); // 5 - Bump Texture, t2
	rootSignature->AddParameter(DX12RootSignature::Type::ConstantBuffer); // 6 - Light Data, b3
	rootSignature->CreateRootSignature(renderInterface->GetDevice());

	// Create Pipeline State
	pipelineStates.PushBack(renderInterface->CreatePipelineState(vertexShader, pixelShader, inputLayout));
	pipelineStates[0]->AddRenderTarget(multiRenderTarget->Get(0)->GetDxgiFormat());
	pipelineStates[0]->CreatePipelineState(renderInterface->GetDevice(), vertexShader->GetBytecode(), pixelShader->GetBytecode(), inputLayout, rootSignature);
	
	pipelineStates.PushBack(renderInterface->CreatePipelineState(vertexShader_noBump, pixelShader_noBump, inputLayout));
	pipelineStates[1]->AddRenderTarget(multiRenderTarget->Get(0)->GetDxgiFormat());
	pipelineStates[1]->CreatePipelineState(renderInterface->GetDevice(), vertexShader->GetBytecode(), pixelShader->GetBytecode(), inputLayout, rootSignature);

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
	//delete renderTarget;
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

void Sapphire::ForwardRenderingPass::Render(DX12CommandList* commandList, RenderInterface* renderInterface, std::vector<GameObject*> objects)
{
	PIXBeginEvent(commandList->GetCommandList(), PIX_COLOR(255, 255, 255), "ForwardRenderingPass");
	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->numOfVertices != 0)
		{
			if (objects[i]->bumpMapWidth == 0)
			{
				commandList->SetPipelineState(pipelineStates[1], rootSignature);
			}
			else
			{
				commandList->SetPipelineState(pipelineStates[0], rootSignature);
			}
			commandList->SetConstantBuffer(1, 16, shadowCamera->GetViewProjectionMatrixPtr());
			commandList->SetConstantBuffer(2, 16, &objects[i]->world);
			commandList->SetTexture(3, renderInterface->GetSrvDescriptor(objects[i]->texture->GetDescriptorIndex()));
			// This is incorrect! I need a descriptr from the Input Resource, not the given Render Target
			commandList->SetTexture(4, renderInterface->GetSrvDescriptor(ShadowMapDepth));
			if (objects[i]->bumpMapWidth != 0)
			{
				commandList->SetTexture(5, renderInterface->GetSrvDescriptor(objects[i]->bumpMap->GetDescriptorIndex()));
			}
			commandList->SetConstantBuffer(6, renderInterface->GetSrvDescriptor(constantBuffer->GetDescriptorIndex()));
			//commandList->Draw(objects[i]->geometry);
			//commandList->Draw(objects[i]->positionVertexBuffer, objects[i]->indexBuffer);
			//commandList->Draw(objects[i]->positionVertexBuffer, objects[i]->normalVertexBuffer, objects[i]->indexBuffer);
			//commandList->Draw(objects[i]->positionVertexBuffer, objects[i]->normalVertexBuffer, objects[i]->colorTexCoordVertexBuffer, objects[i]->indexBuffer);
			commandList->Draw(objects[i]->positionVertexBuffer, objects[i]->normalVertexBuffer, objects[i]->tangentVertexBuffer, objects[i]->colorTexCoordVertexBuffer, objects[i]->indexBuffer);
		}
	}
	PIXEndEvent(commandList->GetCommandList());
}

void Sapphire::ForwardRenderingPass::Render(DX12CommandList* commandList, RenderInterface* renderInterface, std::vector<GameObject*> objects, Camera* shadowMapCamera)
{
	PIXBeginEvent(commandList->GetCommandList(), PIX_COLOR(255, 255, 255), "ForwardRenderingPass");
	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->numOfVertices != 0)
		{
			if (objects[i]->bumpMapWidth == 0)
			{
				commandList->SetPipelineState(pipelineStates[1], rootSignature);
			}
			else
			{
				commandList->SetPipelineState(pipelineStates[0], rootSignature);
			}
			commandList->SetConstantBuffer(1, 16, shadowMapCamera->GetViewProjectionMatrixPtr());
			commandList->SetConstantBuffer(2, 16, &objects[i]->world);
			commandList->SetTexture(3, renderInterface->GetSrvDescriptor(objects[i]->texture->GetDescriptorIndex()));
			// This is incorrect! I need a descriptr from the Input Resource, not the given Render Target
			commandList->SetTexture(4, renderInterface->GetSrvDescriptor(ShadowMapDepth));
			if (objects[i]->bumpMapWidth != 0)
			{
				commandList->SetTexture(5, renderInterface->GetSrvDescriptor(objects[i]->bumpMap->GetDescriptorIndex()));
			}
			commandList->SetConstantBuffer(6, renderInterface->GetSrvDescriptor(constantBuffer->GetDescriptorIndex()));
			//commandList->Draw(objects[i]->geometry);
			//commandList->Draw(objects[i]->positionVertexBuffer, objects[i]->indexBuffer);
			//commandList->Draw(objects[i]->positionVertexBuffer, objects[i]->normalVertexBuffer, objects[i]->indexBuffer);
			//commandList->Draw(objects[i]->positionVertexBuffer, objects[i]->normalVertexBuffer, objects[i]->colorTexCoordVertexBuffer, objects[i]->indexBuffer);
			commandList->Draw(objects[i]->positionVertexBuffer, objects[i]->normalVertexBuffer, objects[i]->tangentVertexBuffer, objects[i]->colorTexCoordVertexBuffer, objects[i]->indexBuffer);
		}
	}
	PIXEndEvent(commandList->GetCommandList());
}

void Sapphire::ForwardRenderingPass::PostRender(DX12CommandList* commandList)
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

void Sapphire::ForwardRenderingPass::SetShadowCamera(Camera* camera)
{
	this->shadowCamera = camera;
}
