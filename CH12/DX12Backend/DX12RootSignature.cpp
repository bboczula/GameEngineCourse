#include "pch.h"

#include "DX12RootSignature.h"

#include "Utils.h"

#include "DX12Device.h"

Sapphire::DX12RootSignature::DX12RootSignature() : srvRootParameterCount(0), shaderRegisterCount(0)
{
}

Sapphire::DX12RootSignature::~DX12RootSignature()
{
	// Here we should delete all the Root Ranges
	for (int i = 0; i < descriptorRanges.size(); i++)
	{
		if (descriptorRanges[i])
		{
			delete[] descriptorRanges[i];
		}
	}
}

void Sapphire::DX12RootSignature::AddParameter(Type type)
{
	D3D12_ROOT_PARAMETER rootParameter;
	switch (type)
	{
	case Sapphire::DX12RootSignature::Matrix:
		rootParameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
		rootParameter.Constants.Num32BitValues = 16;
		rootParameter.Constants.RegisterSpace = 0;
		rootParameter.Constants.ShaderRegister = shaderRegisterCount;
		rootParameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		rootParameters.push_back(rootParameter);
		shaderRegisterCount++;
		break;
	case Sapphire::DX12RootSignature::Texture:
	{
		// This Descriptor Range has to exist during the serialization
		// You can store the pointer, but you would have to create the array on the heap;
		// And then you have to remember to free that memory, might cause leaks
		CD3DX12_DESCRIPTOR_RANGE* range = new CD3DX12_DESCRIPTOR_RANGE[1];
		range[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, srvRootParameterCount);
		// I think this is being copied by value?
		descriptorRanges.push_back(range);

		rootParameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParameter.DescriptorTable.NumDescriptorRanges = 1;
		rootParameter.DescriptorTable.pDescriptorRanges = descriptorRanges.back(); // &range[0];
		rootParameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		rootParameters.push_back(rootParameter);
		srvRootParameterCount++;
		break;
	}
	case Sapphire::DX12RootSignature::ConstantBuffer:
	{
		// This Descriptor Range has to exist during the serialization
		CD3DX12_DESCRIPTOR_RANGE* rangeLight = new CD3DX12_DESCRIPTOR_RANGE[1];
		rangeLight[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, srvRootParameterCount);
		descriptorRanges.push_back(rangeLight);

		rootParameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParameter.DescriptorTable.NumDescriptorRanges = 1;
		rootParameter.DescriptorTable.pDescriptorRanges = descriptorRanges.back(); //&rangeLight[0]; // Array-to-pointer type decay
		rootParameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		rootParameters.push_back(rootParameter);
		srvRootParameterCount++;
		break;
	}
	default:
		break;
	}
}

// Convert a blob to at string
template<class BlotType>
std::string convertBlobToString(BlotType* pBlob)
{
	std::vector<char> infoLog(pBlob->GetBufferSize() + 1);
	memcpy(infoLog.data(), pBlob->GetBufferPointer(), pBlob->GetBufferSize());
	infoLog[pBlob->GetBufferSize()] = 0;
	return std::string(infoLog.data());
}

void Sapphire::DX12RootSignature::CreateRootSignature(DX12Device* device)
{
	// Samplers
	CD3DX12_STATIC_SAMPLER_DESC staticSampler[1];
	staticSampler[0].Init(0, D3D12_FILTER_ANISOTROPIC);

	CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init(this->rootParameters.size(), this->rootParameters.data(), 1, staticSampler, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ID3DBlob* signature;
	ID3DBlob* error;
	auto result  = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
	if (FAILED(result))
	{
		std::string msg = convertBlobToString(error);
		//msgBox(msg);
		return;
	}
	AExitIfFailed(device->GetDevice()->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&rootSignature)));
}

ID3D12RootSignature* Sapphire::DX12RootSignature::GetRootSignature()
{
	return rootSignature;
}
