#include "ImGuiPass.h"
#include "RenderInterface.h"
#include "../DX12Backend/DX12InputLayout.h"
#include "../DX12Backend/DX12RootSignature.h"

#include "EXTERNALS/imgui-master/imgui.h"
#include "EXTERNALS/imgui-master/backends/imgui_impl_dx12.h"
#include "EXTERNALS/imgui-master/backends/imgui_impl_win32.h"

#define USE_PIX
#include "pix3.h"

Sapphire::ImGuiPass::ImGuiPass(RenderInterface* renderInterface, unsigned int width, unsigned int height)
{
	multiRenderTarget = new DX12MultiRenderTarget();
	multiRenderTarget->Add(renderInterface->CreateRenderTarget(GrayscaleRT, width, height));
	depthBuffer = renderInterface->CreateDepthBuffer(width, height);

	// Create Shaders
	vertexShader = new DX12Shader("fullscreen_vs.cso");
	pixelShader = new DX12Shader("grayscale_ps.cso");

	// Create Input Layout (has to be done before PSO, fix that)
	inputLayout = new DX12InputLayout();

	// Need Root Signature, even if empty
	rootSignature = new DX12RootSignature();
	rootSignature->CreateRootSignature(renderInterface->GetDevice());

	pipelineStates.PushBack(renderInterface->CreatePipelineState(vertexShader, pixelShader, inputLayout));
	pipelineStates[0]->AddRenderTarget(multiRenderTarget->Get(0)->GetDxgiFormat());
	pipelineStates[0]->CreatePipelineState(renderInterface->GetDevice(), vertexShader->GetBytecode(), pixelShader->GetBytecode(), inputLayout, rootSignature);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Allocate Descriptor Handles, for what though?
	// You really need to understand how those Descriptor Heaps work in your engine
	auto descriptorIndex = renderInterface->GetSrvDescriptorHeap()->GetHeapSize();
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle;
	srvHandle.ptr = renderInterface->GetSrvDescriptorHeap()->AllocateDescriptor();

	D3D12_GPU_DESCRIPTOR_HANDLE gpuSrvHandle;
	gpuSrvHandle.ptr = renderInterface->GetSrvDescriptorHeap()->GetGpuDescriptor(descriptorIndex);

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(renderInterface->GetHwnd());
	ImGui_ImplDX12_Init(renderInterface->GetDevice()->GetDevice(), 2, DXGI_FORMAT_R8G8B8A8_UNORM,
		renderInterface->GetSrvDescriptorHeap()->GetRawDescriptorHeap(), srvHandle, gpuSrvHandle);
}

Sapphire::ImGuiPass::~ImGuiPass()
{
}

void Sapphire::ImGuiPass::PreRender(DX12CommandList* commandList)
{
}

void Sapphire::ImGuiPass::Render(DX12CommandList* commandList, RenderInterface* renderInterface, std::vector<GameObject*> objects)
{
	PIXBeginEvent(commandList->GetCommandList(), PIX_COLOR(255, 255, 255), "ImGuiPass");
	//commandList->DrawEmpty();
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	bool show_another_window = false;
	ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
	ImGui::Text("Hello from another window!");
	if (ImGui::Button("Close Me"))
		show_another_window = false;
	ImGui::End();

	ImGui::Render();

	// Resource Barrier

	// Add ImGui commands to the command list
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), renderInterface->GetCommandList()->GetCommandList());

	// Resrource Barrier

	PIXEndEvent(commandList->GetCommandList());
}

void Sapphire::ImGuiPass::PostRender(DX12CommandList* commandList)
{
}
