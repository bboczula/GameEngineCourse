#pragma once

#ifdef DX12BACKEND_EXPORTS
#define GPUAPI_DX12_API __declspec(dllexport)
#else
#define GPUAPI_DX12_API __declspec(dllimport)
#endif

#include <Windows.h>
#include <d3d12.h>

namespace Sapphire
{
	// Forward declarations
	class DX12Resource;
	class DX12Device;

	extern class GPUAPI_DX12_API DX12IndexBuffer
	{
		friend class DX12CommandList;
		friend class RenderContext;
	public:
		DX12IndexBuffer(DX12Device* device, void* indexData, UINT indexSize, UINT numOfIndices);
		~DX12IndexBuffer();
		UINT GetNumOfIndices();
	private:
		UINT numOfIndices;
		D3D12_INDEX_BUFFER_VIEW indexBufferView;
		DX12Resource* indexBuffer;
	};
}