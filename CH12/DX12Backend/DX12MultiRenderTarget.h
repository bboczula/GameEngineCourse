#pragma once

#ifdef DX12BACKEND_EXPORTS
#define GPUAPI_DX12_API __declspec(dllexport)
#else
#define GPUAPI_DX12_API __declspec(dllimport)
#endif

#include "DX12RenderTarget.h"

#include <vector>

namespace Sapphire
{
	class DX12RenderTarget;

	class GPUAPI_DX12_API DX12MultiRenderTarget
	{
	public:
		DX12MultiRenderTarget();
		void Add(DX12RenderTarget* renderTarget);
		DX12RenderTarget* Get(unsigned int index);
		unsigned int Size();
		DX12RenderTarget** Data();
		D3D12_CPU_DESCRIPTOR_HANDLE* GetDescriptorHandlesArray();
	private:
		std::vector<DX12RenderTarget*> renderTargets;
		std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> descriptorHandles;
	};
}