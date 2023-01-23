#pragma once

#ifdef DX12BACKEND_EXPORTS
#define GPUAPI_DX12_API __declspec(dllexport)
#else
#define GPUAPI_DX12_API __declspec(dllimport)
#endif

#include <d3d12.h>

namespace Sapphire
{
	class DX12Device;

	class GPUAPI_DX12_API DX12RootSignature
	{
	public:
		DX12RootSignature();
		~DX12RootSignature();
		void CreateRootSignature(DX12Device* device);
		ID3D12RootSignature* GetRootSignature();
	private:
		ID3D12RootSignature* rootSignature;
	};
}