#include "Renderer.h"
#include "RenderContext.h"
#include "Light.h"
#include "ShadowMapPass.h"
#include "ForwardRenderingPass.h"
#include "GrayscalePass.h"
#include "DeferredRenderingPass.h"
#include "LightResolvePass.h"

Sapphire::Renderer::Renderer(HWND hwnd, UINT width, UINT height)
{
	// Create Render Context
	renderContext = new RenderContext(hwnd, width, height);

	// Create global directional light
	directionalLight = new Light(0.0f, 1.0f, 0.0f);

	// Create Shadow Map Pass
	shadowMapPass = new ShadowMapPass(renderContext, directionalLight);

	// Create Forward Rendering Pass
	forwardRenderingPass = new ForwardRenderingPass(renderContext, directionalLight, width, height);

	// Grayscale Render Pass
	grayscalePass = new GrayscalePass(renderContext, width, height);

	// Position Render Pass
	defferedRenderingPass = new DeferredRenderingPass(renderContext, width, height);

	// Light Resolve Pass
	lightResolvePass = new LightResolvePass(renderContext, width, height);

	// Setup the connections
	// Connect resources - this can't be called at runtime
	forwardRenderingPass->AddInputResource(defferedRenderingPass->GetRenderTarget(0)->GetResource());	// Position Texture
	forwardRenderingPass->AddInputResource(defferedRenderingPass->GetRenderTarget(1)->GetResource());	// Normal Texture
	forwardRenderingPass->AddInputResource(defferedRenderingPass->GetRenderTarget(2)->GetResource());	// Albedo Texture
	forwardRenderingPass->AddInputResource(shadowMapPass->GetDepthBuffer()->GetResource());	// Shadow Map Texture
}

Sapphire::Renderer::~Renderer()
{
	delete lightResolvePass;
	delete defferedRenderingPass;
	delete grayscalePass;
	delete shadowMapPass;
	delete forwardRenderingPass;
}

void Sapphire::Renderer::Render(std::vector<GameObject*> objects)
{
	if (directionalLight->GetPositionY() > 0)
	{
		directionalLight->RotateX(0.025f);
	}

	auto commandList = renderContext->GetCommandList();
	commandList->Reset();

	shadowMapPass->Setup(commandList);
	shadowMapPass->PreRender(commandList);
	// Why the line below?
	renderContext->SetSrvDescriptorHeap();
	shadowMapPass->Render(commandList, renderContext, objects);
	shadowMapPass->PostRender(commandList);
	shadowMapPass->Teardown(commandList);

	defferedRenderingPass->Setup(commandList);
	defferedRenderingPass->PreRender(commandList);
	defferedRenderingPass->Render(commandList, renderContext, objects);
	defferedRenderingPass->PostRender(commandList);
	defferedRenderingPass->Teardown(commandList);

	lightResolvePass->Setup(commandList);
	lightResolvePass->PreRender(commandList);
	lightResolvePass->Render(commandList, renderContext, objects);
	lightResolvePass->PostRender(commandList);
	lightResolvePass->Teardown(commandList);

	forwardRenderingPass->Setup(commandList);
	forwardRenderingPass->PreRender(commandList);
	forwardRenderingPass->Render(commandList, renderContext, objects, shadowMapPass->camera);
	forwardRenderingPass->PostRender(commandList);
	forwardRenderingPass->Teardown(commandList);

	grayscalePass->Setup(commandList);
	grayscalePass->PreRender(commandList);
	grayscalePass->Render(commandList, renderContext, objects);
	grayscalePass->PostRender(commandList);
	grayscalePass->Teardown(commandList);

	renderContext->Blit(forwardRenderingPass->GetRenderTarget(0)->GetResource());

	commandList->Close();
}

void Sapphire::Renderer::SetCamera(Camera* camera)
{
	forwardRenderingPass->SetCamera(camera);
	defferedRenderingPass->SetCamera(camera);
}

void Sapphire::Renderer::CreateResources(std::vector<GameObject*> objects)
{
	renderContext->CreateResources(objects);
}

void Sapphire::Renderer::Execute()
{
	renderContext->Execute();
}
