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

	extern class GPUAPI_DX12_API DX12VertexBuffer
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