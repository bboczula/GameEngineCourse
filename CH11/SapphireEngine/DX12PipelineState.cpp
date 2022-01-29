#include "DX12PipelineState.h"

Sapphire::DX12PipelineState::DX12PipelineState(DX12Device* device, DX12Shader* vertexShader, DX12Shader* pixelShader)
{
	Logger::GetInstance().Log("%s\n", "Sapphire::DX12PipelineState::DX12PipelineState()");

	CreateRootSignature(device);
	CreatePipelineState(device, vertexShader->GetBytecode(), pixelShader->GetBytecode());
}

Sapphire::DX12PipelineState::~DX12PipelineState()
{
	Logger::GetInstance().Log("%s\n", "Sapphire::DX12PipelineState::~DX12PipelineState()");

	SafeRelease(&pipelineState);
	SafeRelease(&rootSignature);
}

void Sapphire::DX12PipelineState::CreateRootSignature(DX12Device* device)
{
	Logger::GetInstance().Log("%s\n", "Sapphire::DX12PipelineState::CreateRootSignature()");

	D3D12_ROOT_PARAMETER rootParameters[2];
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	rootParameters[0].Constants.Num32BitValues = 16;
	rootParameters[0].Constants.RegisterSpace = 0;
	rootParameters[0].Constants.ShaderRegister = 0;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	rootParameters[1].Constants.Num32BitValues = 16;
	rootParameters[1].Constants.RegisterSpace = 0;
	rootParameters[1].Constants.ShaderRegister = 1;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init(2, rootParameters, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ID3DBlob* signature;
	ID3DBlob* error;
	ExitIfFailed(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error));
	ExitIfFailed(device->GetDevice()->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&rootSignature)));
}

void Sapphire::DX12PipelineState::CreatePipelineState(DX12Device* device, D3D12_SHADER_BYTECODE vs, D3D12_SHADER_BYTECODE ps)
{
	Logger::GetInstance().Log("%s\n", "Sapphire::DX12PipelineState::CreatePipelineState()");

	D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
	ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
	psoDesc.pRootSignature = rootSignature;
	psoDesc.VS = vs;
	psoDesc.PS = ps;
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psoDesc.SampleDesc.Count = 1;

	ExitIfFailed(device->GetDevice()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pipelineState)));
}
