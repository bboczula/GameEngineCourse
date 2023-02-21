#include "ShadowMapPass.h"

#include "RenderContext.h"
#include "OrthographicCamera.h"
#include "../DX12Backend/DX12InputLayout.h"
#include "../DX12Backend/DX12RenderTarget.h"
#include "../DX12Backend/DX12RootSignature.h"
#include "Light.h"
#include "Arcball.h"

#define USE_PIX
#include "pix3.h"

Sapphire::ShadowMapPass::ShadowMapPass(RenderContext* renderContext, Light* light) : light(light)
{
	depthBuffer = renderContext->CreateDepthBufferWithSrv(ShadowMapDepth, 2048, 2048);
	multiRenderTarget = new DX12MultiRenderTarget();
	multiRenderTarget->Add(renderContext->CreateRenderTarget(ShadowMapRT, 2048, 2048));

	// Create Shaders
	vertexShader = new DX12Shader("shadow_vs.cso");
	pixelShader = new DX12Shader("shadow_ps.cso");

	// Create Input Layout
	inputLayout = new DX12InputLayout();
	inputLayout->AppendElementT(VertexStream::Position);

	// Need Root Signature
	rootSignature = new DX12RootSignature();
	rootSignature->AddParameter(DX12RootSignature::Type::Matrix); // 0 - View Projection Matrix
	rootSignature->AddParameter(DX12RootSignature::Type::Matrix); // 1 - World Matrix
	rootSignature->CreateRootSignature(renderContext->GetDevice());

	// Create Pipeline State
	pipelineStates.PushBack(renderContext->CreatePipelineState(vertexShader, pixelShader, inputLayout));
	pipelineStates[0]->AddRenderTarget(multiRenderTarget->Get(0)->GetDxgiFormat());
	pipelineStates[0]->CreatePipelineState(renderContext->GetDevice(), vertexShader->GetBytecode(), pixelShader->GetBytecode(), inputLayout, rootSignature);

	// Create Camera
	//camera = new Camera(1280.0f / 720.0f);
	camera = new OrthographicCamera({ 0.0f, 0.0f, 0.0f });

	// In this pass camera is fixed
	const float scaleFactor = 20.0f;
	//camera->SetPosition({ scaleFactor * light->GetPositionX(), scaleFactor * light->GetPositionY(), scaleFactor * light->GetPositionZ() });
	//camera->SetTarget({ 0.0f, 19.0f, 0.0f });
	//camera->SetUp({ 0.0f, 0.0f, 1.0f });
	//camera->DoIt();

	arcball = new Arcball(camera);
	arcball->Rotate(-90, 0, 0);
}

Sapphire::ShadowMapPass::~ShadowMapPass()
{
	delete depthBuffer;
}

void Sapphire::ShadowMapPass::PreRender(DX12CommandList* commandList)
{
	// In this pass camera is fixed
	//const float scaleFactor = 20.0f;
	//camera->SetPosition({ scaleFactor * light->GetPositionX(), scaleFactor * light->GetPositionY(), scaleFactor * light->GetPositionZ() });
	//camera->SetTarget({ 0.0f, 19.0f, 0.0f });
	//camera->SetUp({ 0.0f, 0.0f, 1.0f });
	//camera->DoIt();
	arcball->Rotate(light->GetRotationX(), 0.0f, 0.0f);

	const float clearColor[] = { 0.1176f, 0.1882f, 0.4470f, 1.0f };
	//unsigned int currentFrameIndex = deviceContext->GetCurrentFrameIndex();

	//commandList->Reset();
	//commandList->TransitionTo(renderTarget->GetResource(), D3D12_RESOURCE_STATE_RENDER_TARGET);
	//commandList->SetPipelineState(dxPipelineState);
	// commandList->SetRenderTarget(renderTarget, depthBuffer);
	// commandList->ClearRenderTarget(renderTarget, clearColor);
	// Do I have to manage resource states?
	// commandList->ClearDepthBuffer(depthBuffer);
	commandList->SetConstantBuffer(0, 16, camera->GetViewProjectionMatrixPtr());
	//commandList->SetConstantBuffer(1, 16, camera->GetProjectionMatrixPtr());
}

void Sapphire::ShadowMapPass::Render(DX12CommandList* commandList, RenderContext* renderContext, std::vector<GameObject*> objects)
{
	PIXBeginEvent(commandList->GetCommandList(), PIX_COLOR(255, 255, 255), "ShadowMapPass");
	// This could potentially be Render Pass
	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->numOfVertices != 0)
		{
			commandList->SetConstantBuffer(1, 16, &objects[i]->world);
			// D3D12_GPU_DESCRIPTOR_HANDLE descriptor;
			// descriptor.ptr = srvDescriptorHeap->GetFirstGpuDescriptor().ptr + i * srvDescriptorHeap->GetDescriptorSize();
			// commandList->SetTexture(3, descriptor);
			//commandList->SetTexture(3, renderContext->GetSrvDescriptor(i));
			//commandList->Draw(objects[i]->geometry);
			commandList->Draw(objects[i]->positionVertexBuffer, objects[i]->indexBuffer);
			//commandList->Draw(objects[i]->positionVertexBuffer, objects[i]->normalVertexBuffer, objects[i]->indexBuffer);
			//commandList->Draw(objects[i]->positionVertexBuffer, objects[i]->normalVertexBuffer, objects[i]->colorTexCoordVertexBuffer, objects[i]->indexBuffer);
		}
	}
	PIXEndEvent(commandList->GetCommandList());
}

void Sapphire::ShadowMapPass::PostRender(DX12CommandList* commandList)
{
}

Sapphire::DX12DepthBuffer* Sapphire::ShadowMapPass::GetDepthBuffer()
{
	return depthBuffer;
}
