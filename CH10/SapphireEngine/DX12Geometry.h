#pragma once

#include "DX12Device.h"
#include "Utils.h"
#include "d3dx12.h"
#include "DX12Resource.h"

#include <d3d12.h>

namespace Sapphire
{
	class DX12Geometry
	{
		friend class DX12CommandList;
	public:
		DX12Geometry(DX12Device* device, void* triangleVertices, UINT vertexBufferSize, UINT vertexSize, UINT numOfVertices);
		~DX12Geometry();

	private:
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
		DX12Resource* vertexBuffer;
	};
}