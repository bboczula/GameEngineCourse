#include "pch.h"

#include "DX12PipelineState.h"
#include "DX12InputLayout.h"
#include "DX12Shader.h"
#include "DX12Device.h"
#include "DX12RootSignature.h"

Sapphire::DX12PipelineState::DX12PipelineState(DX12Device* device)
{
}

void Sapphire::DX12PipelineState::AddRenderTarget(DXGI_FORMAT format)
{
	rtFormats.push_back(format);
}

Sapphire::DX12PipelineState::~DX12PipelineState()
{
	ASafeRelease(&pipelineState);
}

void Sapphire::DX12PipelineState::CreatePipelineState(DX12Device* device, D3D12_SHADER_BYTECODE vs, D3D12_SHADER_BYTECODE ps, DX12InputLayout* inputLayout, DX12RootSignature* rootSignature)
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
	ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	psoDesc.InputLayout = inputLayout->GetInputLayoutDesc();
	psoDesc.pRootSignature = rootSignature->GetRootSignature();
	psoDesc.VS = vs;
	psoDesc.PS = ps;
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.SampleDesc.Count = 1;
	psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// Fill Render Target formats
	psoDesc.NumRenderTargets = rtFormats.size();
	for (int i = 0; i < psoDesc.NumRenderTargets; i++)
	{
		psoDesc.RTVFormats[i] = rtFormats[i];
		psoDesc.BlendState.RenderTarget[i].BlendEnable = TRUE;
		psoDesc.BlendState.RenderTarget[i].SrcBlend = D3D12_BLEND_SRC_ALPHA;
		//psoDesc.BlendState.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ZERO;
		psoDesc.BlendState.RenderTarget[i].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		//psoDesc.BlendState.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
		psoDesc.BlendState.RenderTarget[i].BlendOpAlpha = D3D12_BLEND_OP_ADD;
		//psoDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = 0x0F;
	}

	AExitIfFailed(device->GetDevice()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pipelineState)));
}
