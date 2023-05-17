#include "ShadowMapPass.h"

#include "RenderInterface.h"
#include "OrthographicCamera.h"
#include "../DX12Backend/DX12InputLayout.h"
#include "../DX12Backend/DX12RenderTarget.h"
#include "../DX12Backend/DX12RootSignature.h"
#include "Arcball.h"
#include "LightObject.h"

#define USE_PIX
#include "pix3.h"

Sapphire::ShadowMapPass::ShadowMapPass(RenderInterface* renderInterface)
{
	depthBuffer = renderInterface->CreateDepthBufferWithSrv(ShadowMapDepth, 2048, 2048);
	multiRenderTarget = new DX12MultiRenderTarget();
	multiRenderTarget->Add(renderInterface->CreateRenderTarget(ShadowMapRT, 2048, 2048));

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
	rootSignature->CreateRootSignature(renderInterface->GetDevice());

	// Create Pipeline State
	pipelineStates.PushBack(renderInterface->CreatePipelineState(vertexShader, pixelShader, inputLayout));
	pipelineStates[0]->AddRenderTarget(multiRenderTarget->Get(0)->GetDxgiFormat());
	pipelineStates[0]->CreatePipelineState(renderInterface->GetDevice(), vertexShader->GetBytecode(), pixelShader->GetBytecode(), inputLayout, rootSignature);

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
}

void Sapphire::ShadowMapPass::Render(DX12CommandList* commandList, RenderInterface* renderInterface, std::vector<GameObject*> objects, std::vector<LightObject*> lights)
{
	PIXBeginEvent(commandList->GetCommandList(), PIX_COLOR(255, 255, 255), "ShadowMapPass");

	// The only thing i need from the light is an angle to setup the camera
	arcball->Rotate(lights[0]->GetRotationX(), lights[0]->GetRotationY(), lights[0]->GetRotationZ());
	commandList->SetConstantBuffer(0, 16, camera->GetViewProjectionMatrixPtr());

	// This could potentially be Render Pass
	for (int i = 0; i < objects.size(); i++)
	{
		if (!objects[i]->metaIsVisible.value)
		{
			continue;
		}
		if (objects[i]->numOfVertices != 0)
		{
			commandList->SetConstantBuffer(1, 16, &objects[i]->world);
			commandList->Draw(objects[i]->positionVertexBuffer, objects[i]->indexBuffer);
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
