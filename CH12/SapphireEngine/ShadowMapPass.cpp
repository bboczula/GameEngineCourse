#include "ShadowMapPass.h"

#include "RenderContext.h"
#include "OrthographicCamera.h"
#include "DX12InputLayout.h"

Sapphire::ShadowMapPass::ShadowMapPass(DeviceContext* deviceContext, RenderContext* renderContext)
{
	renderTarget = renderContext->CreateRenderTarget(deviceContext, 1024, 1024);
	depthBuffer = renderContext->CreateDepthBufferWithSrv(deviceContext, 1024, 1024);

	// Create Shaders
	vertexShader = new DX12Shader("shadow_vs.cso");
	pixelShader = new DX12Shader("shadow_ps.cso");

	// Create Input Layout
	inputLayout = new DX12InputLayout();
	inputLayout->AppendElement(VertexStream::Position);

	// Create Pipeline State
	dxPipelineState = new DX12PipelineState(deviceContext->GetDevice(), vertexShader, pixelShader, inputLayout);
	viewport = new DX12Viewport(1280, 720);

	// Create Camera
	//camera = new Camera(1280.0f / 720.0f);
	camera = new OrthographicCamera();

	// In this pass camera is fixed
	camera->SetPosition({ 0.0f, 10.0f, 5.0f });
	//camera->SetTarget({ 0.0f, 19.0f, 0.0f });
	//camera->SetUp({ 0.0f, 0.0f, 1.0f });
	camera->DoIt();
}

Sapphire::ShadowMapPass::~ShadowMapPass()
{
	delete depthBuffer;
	delete renderTarget;
}

void Sapphire::ShadowMapPass::Setup(DX12CommandList* commandList)
{
	const float clearColor[] = { 0.1176f, 0.1882f, 0.4470f, 1.0f };
	//unsigned int currentFrameIndex = deviceContext->GetCurrentFrameIndex();

	commandList->Reset();
	//commandList->TransitionTo(renderTarget->GetResource(), D3D12_RESOURCE_STATE_RENDER_TARGET);
	commandList->SetPipelineState(dxPipelineState);
	commandList->SetRenderTarget(renderTarget, depthBuffer);
	commandList->ClearRenderTarget(renderTarget, clearColor);
	// Do I have to manage resource states?
	commandList->ClearDepthBuffer(depthBuffer);
	commandList->SetConstantBuffer(0, 16, camera->GetViewProjectionMatrixPtr());
	//commandList->SetConstantBuffer(1, 16, camera->GetProjectionMatrixPtr());
}

void Sapphire::ShadowMapPass::Render(DX12CommandList* commandList, RenderContext* renderContext, std::vector<GameObject*> objects)
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
			//commandList->SetTexture(3, renderContext->GetSrvDescriptor(i));
			//commandList->Draw(objects[i]->geometry);
			commandList->Draw(objects[i]->positionVertexBuffer, objects[i]->indexBuffer);
			//commandList->Draw(objects[i]->positionVertexBuffer, objects[i]->normalVertexBuffer, objects[i]->indexBuffer);
			//commandList->Draw(objects[i]->positionVertexBuffer, objects[i]->normalVertexBuffer, objects[i]->colorTexCoordVertexBuffer, objects[i]->indexBuffer);
		}
	}
}

void Sapphire::ShadowMapPass::Teardown(DX12CommandList* commandList)
{
	//commandList->Close();
}

Sapphire::DX12DepthBuffer* Sapphire::ShadowMapPass::GetDepthBuffer()
{
	return depthBuffer;
}
