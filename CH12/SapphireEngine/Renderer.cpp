#include "Renderer.h"
#include "RenderInterface.h"
#include "ShadowMapPass.h"
#include "ForwardRenderingPass.h"
#include "GrayscalePass.h"
#include "DeferredRenderingPass.h"
#include "LightResolvePass.h"
#include "ImGuiPass.h"
#include "LightObject.h"
#include "GameObject.h"

Sapphire::Renderer::Renderer(HWND hwnd, UINT width, UINT height)
{
	// Create Render Context
	renderInterface = new RenderInterface(hwnd, width, height);

	// Create Shadow Map Pass
	shadowMapPass = new ShadowMapPass(renderInterface);

	// Create Forward Rendering Pass
	forwardRenderingPass = new ForwardRenderingPass(renderInterface, width, height);

	// Grayscale Render Pass
	grayscalePass = new GrayscalePass(renderInterface, width, height);

	// Position Render Pass
	defferedRenderingPass = new DeferredRenderingPass(renderInterface, width, height);

	// Light Resolve Pass
	lightResolvePass = new LightResolvePass(renderInterface, width, height);

	// Im GUI Pass
	imGuiPass = new ImGuiPass(renderInterface, width, height, forwardRenderingPass->GetMultiRenderTarget());

	// Setup the connections
	// Connect resources - this can't be called at runtime
	forwardRenderingPass->AddInputResource(defferedRenderingPass->GetRenderTarget(0)->GetResource());	// Position Texture
	forwardRenderingPass->AddInputResource(defferedRenderingPass->GetRenderTarget(1)->GetResource());	// Normal Texture
	forwardRenderingPass->AddInputResource(defferedRenderingPass->GetRenderTarget(2)->GetResource());	// Albedo Texture
	forwardRenderingPass->AddInputResource(shadowMapPass->GetDepthBuffer()->GetResource());	// Shadow Map Texture

	// Fill the vector for automatic execution
	// This also determines the execution order, since this is all single threaded
	renderPasses.push_back(shadowMapPass);
	renderPasses.push_back(forwardRenderingPass);
	renderPasses.push_back(grayscalePass);
	renderPasses.push_back(defferedRenderingPass);
	renderPasses.push_back(lightResolvePass);
	renderPasses.push_back(imGuiPass);

	forwardRenderingPass->SetShadowCamera(shadowMapPass->camera);
}

Sapphire::Renderer::~Renderer()
{
	delete lightResolvePass;
	delete defferedRenderingPass;
	delete grayscalePass;
	delete shadowMapPass;
	delete forwardRenderingPass;
}

void Sapphire::Renderer::Render(std::vector<GameObject*> objects, std::vector<LightObject*> lights)
{
	auto commandList = renderInterface->GetCommandList();
	commandList->Reset();

	renderInterface->SetSrvDescriptorHeap();

	// Say I want to rotate it
	//lights[0]->RotateX(0.25f);

	for (int i = 0; i < renderPasses.size(); i++)
	{
		renderPasses[i]->Setup(commandList);
		renderPasses[i]->PreRender(commandList);
		renderPasses[i]->Render(commandList, renderInterface, objects, lights);
		renderPasses[i]->PostRender(commandList);
		renderPasses[i]->Teardown(commandList);
	}

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
