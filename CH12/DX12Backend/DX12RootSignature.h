#pragma once

#ifdef DX12BACKEND_EXPORTS
#define GPUAPI_DX12_API __declspec(dllexport)
#else
#define GPUAPI_DX12_API __declspec(dllimport)
#endif

#include <d3d12.h>
#include <vector>
#include "d3dx12.h"

namespace Sapphire
{
	class DX12Device;

	class GPUAPI_DX12_API DX12RootSignature
	{
	public:
		enum Type
		{
			Matrix,
			Texture,
			ConstantBuffer
		};
		DX12RootSignature();
		~DX12RootSignature();
		void AddParameter(Type type);
		void CreateRootSignature(DX12Device* device);
		ID3D12RootSignature* GetRootSignature();
	private:
		std::vector<D3D12_ROOT_PARAMETER> rootParameters = {};
		std::vector<CD3DX12_DESCRIPTOR_RANGE*> descriptorRanges = {};
		ID3D12RootSignature* rootSignature;
		UINT srvRootParameterCount;
		UINT shaderRegisterCount;
	};
}