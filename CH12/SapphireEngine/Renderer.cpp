#include "Renderer.h"
#include "RenderInterface.h"
#include "Light.h"
#include "ShadowMapPass.h"
#include "ForwardRenderingPass.h"
#include "GrayscalePass.h"
#include "DeferredRenderingPass.h"
#include "LightResolvePass.h"

Sapphire::Renderer::Renderer(HWND hwnd, UINT width, UINT height)
{
	// Create Render Context
	renderInterface = new RenderInterface(hwnd, width, height);

	// Create global directional light
	directionalLight = new Light(0.0f, 1.0f, 0.0f);

	// Create Shadow Map Pass
	shadowMapPass = new ShadowMapPass(renderInterface, directionalLight);

	// Create Forward Rendering Pass
	forwardRenderingPass = new ForwardRenderingPass(renderInterface, directionalLight, width, height);

	// Grayscale Render Pass
	grayscalePass = new GrayscalePass(renderInterface, width, height);

	// Position Render Pass
	defferedRenderingPass = new DeferredRenderingPass(renderInterface, width, height);

	// Light Resolve Pass
	lightResolvePass = new LightResolvePass(renderInterface, width, height);

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

	auto commandList = renderInterface->GetCommandList();
	commandList->Reset();

	shadowMapPass->Setup(commandList);
	shadowMapPass->PreRender(commandList);
	// Why the line below?
	renderInterface->SetSrvDescriptorHeap();
	shadowMapPass->Render(commandList, renderInterface, objects);
	shadowMapPass->PostRender(commandList);
	shadowMapPass->Teardown(commandList);

	defferedRenderingPass->Setup(commandList);
	defferedRenderingPass->PreRender(commandList);
	defferedRenderingPass->Render(commandList, renderInterface, objects);
	defferedRenderingPass->PostRender(commandList);
	defferedRenderingPass->Teardown(commandList);

	lightResolvePass->Setup(commandList);
	lightResolvePass->PreRender(commandList);
	lightResolvePass->Render(commandList, renderInterface, objects);
	lightResolvePass->PostRender(commandList);
	lightResolvePass->Teardown(commandList);

	forwardRenderingPass->Setup(commandList);
	forwardRenderingPass->PreRender(commandList);
	forwardRenderingPass->Render(commandList, renderInterface, objects, shadowMapPass->camera);
	forwardRenderingPass->PostRender(commandList);
	forwardRenderingPass->Teardown(commandList);

	grayscalePass->Setup(commandList);
	grayscalePass->PreRender(commandList);
	grayscalePass->Render(commandList, renderInterface, objects);
	grayscalePass->PostRender(commandList);
	grayscalePass->Teardown(commandList);

	renderInterface->Blit(forwardRenderingPass->GetRenderTarget(0)->GetResource());

	commandList->Close();
}

void Sapphire::Renderer::SetCamera(Camera* camera)
{
	forwardRenderingPass->SetCamera(camera);
	defferedRenderingPass->SetCamera(camera);
}

void Sapphire::Renderer::CreateResources(std::vector<GameObject*> objects)
{
	renderInterface->CreateResources(objects);
}

void Sapphire::Renderer::Execute()
{
	renderInterface->Execute();
}
