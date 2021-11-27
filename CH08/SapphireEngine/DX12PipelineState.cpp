#include "DX12PipelineState.h"

Sapphire::DX12PipelineState::DX12PipelineState(ID3D12Device* device, LPCWSTR shaderFileName)
{
	CreateAndCompileShaders(shaderFileName);
	CreateRootSignature(device);
	CreatePipelineState(device);
}

Sapphire::DX12PipelineState::~DX12PipelineState()
{
}

void Sapphire::DX12PipelineState::CreateAndCompileShaders(LPCWSTR shaderFileName)
{
	Logger::GetInstance().Log("%s\n", "Sapphire::DX12PipelineState::CreateAndCompileShaders()");

	vs = new DX12Shader(shaderFileName, L"VSMain", L"vs_6_0");
	ps = new DX12Shader(shaderFileName, L"PSMain", L"ps_6_0");
}

void Sapphire::DX12PipelineState::CreateRootSignature(ID3D12Device* device)
{
	Logger::GetInstance().Log("%s\n", "Sapphire::DX12PipelineState::CreateRootSignature()");

	CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ID3DBlob* signature;
	ID3DBlob* error;
	ExitIfFailed(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error));
	ExitIfFailed(device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&rootSignature)));
}

void Sapphire::DX12PipelineState::CreatePipelineState(ID3D12Device* device)
{
	Logger::GetInstance().Log("%s\n", "Sapphire::DX12PipelineState::CreatePipelineState()");

	// Define the vertex input layout.
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	// Modify the Depth State a little to avoid warning
	auto disabledDepthStencil = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	disabledDepthStencil.DepthEnable = FALSE;

	// Describe and create the graphics pipeline state object (PSO).
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
	ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
	psoDesc.pRootSignature = rootSignature;
	psoDesc.VS = vs->GetBytecode();
	psoDesc.PS = ps->GetBytecode();
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState = disabledDepthStencil;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psoDesc.SampleDesc.Count = 1;

	ExitIfFailed(device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pipelineState)));
}
