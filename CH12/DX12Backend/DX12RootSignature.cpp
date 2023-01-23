#include "pch.h"

#include "DX12RootSignature.h"

#include "d3dx12.h"
#include "Utils.h"

#include "DX12Device.h"

Sapphire::DX12RootSignature::DX12RootSignature()
{
}

Sapphire::DX12RootSignature::~DX12RootSignature()
{
}

void Sapphire::DX12RootSignature::CreateRootSignature(DX12Device* device)
{
	// This has to be automated somehow. This could be a vector
	D3D12_ROOT_PARAMETER rootParameters[7];
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

	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	rootParameters[2].Constants.Num32BitValues = 16;
	rootParameters[2].Constants.RegisterSpace = 0;
	rootParameters[2].Constants.ShaderRegister = 2;
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	// Light Source Matrices
	// rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	// rootParameters[3].Constants.Num32BitValues = 16;
	// rootParameters[3].Constants.RegisterSpace = 0;
	// rootParameters[3].Constants.ShaderRegister = 3;
	// rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	// 
	// rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	// rootParameters[4].Constants.Num32BitValues = 16;
	// rootParameters[4].Constants.RegisterSpace = 0;
	// rootParameters[4].Constants.ShaderRegister = 4;
	// rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	CD3DX12_DESCRIPTOR_RANGE range[1];
	range[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	// Color Texture
	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[3].DescriptorTable.NumDescriptorRanges = 1;
	rootParameters[3].DescriptorTable.pDescriptorRanges = &range[0];
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	// Depth Buffer as SRV
	CD3DX12_DESCRIPTOR_RANGE rangeDepth[1];
	rangeDepth[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);

	rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[4].DescriptorTable.NumDescriptorRanges = 1;
	rootParameters[4].DescriptorTable.pDescriptorRanges = &rangeDepth[0];
	rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	// Bump Map Texture
	CD3DX12_DESCRIPTOR_RANGE rangeBump[1];
	rangeBump[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 2);
	rootParameters[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[5].DescriptorTable.NumDescriptorRanges = 1;
	rootParameters[5].DescriptorTable.pDescriptorRanges = &rangeBump[0];
	rootParameters[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	// Light Position Constant Buffer
	CD3DX12_DESCRIPTOR_RANGE rangeLight[1];
	rangeLight[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 3);

	rootParameters[6].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[6].DescriptorTable.NumDescriptorRanges = 1;
	rootParameters[6].DescriptorTable.pDescriptorRanges = &rangeLight[0];
	rootParameters[6].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	// Samplers
	CD3DX12_STATIC_SAMPLER_DESC staticSampler[1];
	staticSampler[0].Init(0, D3D12_FILTER_ANISOTROPIC);

	CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init(7, rootParameters, 1, staticSampler, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ID3DBlob* signature;
	ID3DBlob* error;
	AExitIfFailed(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error));
	AExitIfFailed(device->GetDevice()->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&rootSignature)));
}

ID3D12RootSignature* Sapphire::DX12RootSignature::GetRootSignature()
{
	return rootSignature;
}
