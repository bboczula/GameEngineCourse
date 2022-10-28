#pragma once

#include <d3d12.h>

namespace GpuApi
{
	class DX12Viewport
	{
		friend class DX12CommandList;
	public:
		DX12Viewport(UINT width, UINT height);
		~DX12Viewport();
	private:
		D3D12_VIEWPORT viewport;
		D3D12_RECT scissors;
	};
}