#pragma once

#ifdef DX12BACKEND_EXPORTS
#define GPUAPI_DX12_API __declspec(dllexport)
#else
#define GPUAPI_DX12_API __declspec(dllimport)
#endif

#include <d3d12.h>
#include <dxgi1_5.h>

#pragma comment(lib, "d3d12.lib")

// You can declare C++ classes with the dllimport or dllexport attribute.
// These forms imply that the entire class is imported or exported.
// Classes exported this way are called exportable classes.

// For DLLs, a.Lib file is not created if the DLL exports nothing for external usage.

namespace Sapphire
{
	class GPUAPI_DX12_API DX12Device
	{
	public:
		DX12Device(IDXGIAdapter1* adapter);
		~DX12Device();
		ID3D12Device* GetDevice();
	private:
		void EnableDebugLayer();
		ID3D12Device* device;
	};
}