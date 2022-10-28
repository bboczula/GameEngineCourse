#pragma once

#include <Windows.h>
#include <d3d12.h>

namespace GpuApi
{
	// Forward declarations
	class DX12Resource;
	class DX12Device;

	class DX12VertexBuffer
	{
		friend class DX12CommandList;
		friend class RenderContext;
	public:
		DX12VertexBuffer(DX12Device* device, void* vertexData, UINT vertexSize, UINT numOfVertices);
		~DX12VertexBuffer();
	private:
		UINT numOfVertices;
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
		DX12Resource* positionVertexBuffer;
	};
}