#pragma once

#ifdef DX12BACKEND_EXPORTS
#define GPUAPI_DX12_API __declspec(dllexport)
#else
#define GPUAPI_DX12_API __declspec(dllimport)
#endif

#include <d3d12.h>

namespace Sapphire
{
	class GPUAPI_DX12_API DX12Viewport
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