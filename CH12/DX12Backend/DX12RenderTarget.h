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
	class DX12Resource;
	class DX12Viewport;

	/// <summary>
	/// This class would be an equivalend of the Render Target View class, if one existed in DirectX 12.
	/// </summary>
	class GPUAPI_DX12_API DX12RenderTarget
	{
		friend class DX12CommandList;
		friend class DX12MultiRenderTarget;
	public:
		enum Format
		{
			RGBA8_UNORM,
			RGBA16_FLOAT

		};

		/// <summary>
		/// Creates new Render Target instance and stores pointers to associated resource and an empty descriptor handle.
		/// </summary>
		/// <param name="device">DirectX 12 device used to create new Render Target</param>
		/// <param name="resource">A resource associated with the Render Target</param>
		/// <param name="descriptor">A Descriptor Handle to use during Render Target creation</param>
		DX12RenderTarget(DX12Device* device, DX12Resource* resource, D3D12_CPU_DESCRIPTOR_HANDLE descriptor);

		/// <summary>
		/// Creates new Render Target with underlying Texture resource.
		/// </summary>
		/// <param name="device"></param>
		/// <param name="descriptor"></param>
		/// <param name="width"></param>
		/// <param name="height"></param>
		DX12RenderTarget(DX12Device* device, D3D12_CPU_DESCRIPTOR_HANDLE descriptor, UINT width, UINT height, Format format = Format::RGBA8_UNORM);

		DX12Resource* GetResource();

		~DX12RenderTarget();

	private:
		DX12Resource* resource;
		D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle;
		DX12Viewport* viewport;
	};
}