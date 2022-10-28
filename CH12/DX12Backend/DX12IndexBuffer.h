#pragma once

#include <Windows.h>
#include <d3d12.h>

namespace GpuApi
{
	// Forward declarations
	class DX12Resource;
	class DX12Device;

	class DX12IndexBuffer
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