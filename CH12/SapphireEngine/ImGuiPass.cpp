#include "ImGuiPass.h"
#include "RenderInterface.h"
#include "../DX12Backend/DX12InputLayout.h"
#include "../DX12Backend/DX12RootSignature.h"

#include "EXTERNALS/imgui-master/imgui.h"
#include "EXTERNALS/imgui-master/backends/imgui_impl_dx12.h"
#include "EXTERNALS/imgui-master/backends/imgui_impl_win32.h"

#define USE_PIX
#include "pix3.h"

Sapphire::ImGuiPass::ImGuiPass(RenderInterface* renderInterface, unsigned int width, unsigned int height, DX12MultiRenderTarget* multiRenderTarget)
{
	// multiRenderTarget = new DX12MultiRenderTarget();
	// multiRenderTarget->Add(renderInterface->CreateRenderTarget(GrayscaleRT, width, height));
	this->multiRenderTarget = multiRenderTarget;
	depthBuffer = renderInterface->CreateDepthBuffer(width, height);
	clearRenderTargets = false;

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
	//io.ConfigFlags |= ImGuiConfigFlags_NoMouse;

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

	SetupGlobalStyle();

	ImGui::NewFrame();
	bool show_another_window = false;
	ImGui::SetNextWindowPos(ImVec2(0, 0), 0, ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(400, 600), ImGuiCond_Once);
	ImGui::Begin("Game Object Tree", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
	static int selection_mask = (1 << 2);
	static int node_clicked = -1;
	for (int i = 0; i < objects.size(); i++)
	{
		// Need to reset flags for every item
		auto flags = ImGuiTreeNodeFlags_FramePadding | /*ImGuiTreeNodeFlags_OpenOnArrow |*/ ImGuiTreeNodeFlags_SpanAvailWidth;
		const bool is_selected = (selection_mask & (1 << i)) != 0;
		if (is_selected)
			flags |= ImGuiTreeNodeFlags_Selected;
		//ImGui::PushID(index);
		boolean treeNodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)i, flags, objects[i]->name.c_str());
		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
			node_clicked = i;
		if (treeNodeOpen)
		{
			// Prevent this infinite tree thingy
			ImGui::TreePop();
		}
	}

	if (node_clicked != -1)
	{
		// Update selection state
		// (process outside of tree loop to avoid visual inconsistencies during the clicking frame)
		if (ImGui::GetIO().KeyCtrl)
			selection_mask ^= (1 << node_clicked);          // CTRL+click to toggle
		else //if (!(selection_mask & (1 << node_clicked))) // Depending on selection behavior you want, may want to preserve selection when clicking on item that is part of the selection
			selection_mask = (1 << node_clicked);           // Click to single-select
	}
	//if (align_label_with_current_x_position)
	//	ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
	ImGui::TreePop();
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(0, 600), 0, ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiCond_Once);
	ImGui::Begin("Game Object Properties", &show_another_window);
	if (node_clicked >= 0)
	{
		bool isVisible = objects[node_clicked]->GetIsVisible();
		ImGui::Checkbox("IsVisible: ", &isVisible);
		objects[node_clicked]->SetIsVisible(isVisible);
		ImGui::Text("Name: %s", objects[node_clicked]->name.c_str());
		ImGui::Text("Indices: %d", objects[node_clicked]->numOfIndices);
		ImGui::Text("Vertices: %d", objects[node_clicked]->numOfVertices);
		ImGui::Text("Position: (%f, %f, %f)", objects[node_clicked]->position->x, objects[node_clicked]->position->y, objects[node_clicked]->position->z);
		ImGui::Text("Color Texture: %d x %d", objects[node_clicked]->textureWidth, objects[node_clicked]->textureHeight);
		ImGui::Text("Bump Texture: %d x %d", objects[node_clicked]->bumpMapWidth, objects[node_clicked]->bumpMapHeight);
	}
	ImGui::End();;

	ImGui::Render();

	ImGui::EndFrame();

	// Resource Barrier

	// Add ImGui commands to the command list
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), renderInterface->GetCommandList()->GetCommandList());

	// Resrource Barrier


	PIXEndEvent(commandList->GetCommandList());
}

void Sapphire::ImGuiPass::SetupGlobalStyle()
{
	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.08f, 0.50f, 0.72f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
	style.Colors[ImGuiCol_Separator] = style.Colors[ImGuiCol_Border];
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.41f, 0.42f, 0.44f, 1.00f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.29f, 0.30f, 0.31f, 0.67f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.08f, 0.08f, 0.09f, 0.83f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.33f, 0.34f, 0.36f, 0.83f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.23f, 0.23f, 0.24f, 1.00f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
	//style.Colors[ImGuiCol_DockingPreview] = ImVec4(0.26f, 0.59f, 0.98f, 0.70f);
	//style.Colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	style.GrabRounding = style.FrameRounding = 2.3f;
}

void Sapphire::ImGuiPass::PostRender(DX12CommandList* commandList)
{
}
