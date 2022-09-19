#pragma once

#include "Utils.h"
#include "Logger.h"
#include "DX12Device.h"
#include "DX12Resource.h"
#include "DX12Viewport.h"

#include <d3d12.h>

namespace Sapphire
{
	/// <summary>
	/// This class would be an equivalend of the Render Target View class, if one existed in DirectX 12.
	/// </summary>
	class DX12RenderTarget
	{
		friend class DX12CommandList;
	public:
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
		DX12RenderTarget(DX12Device* device, D3D12_CPU_DESCRIPTOR_HANDLE descriptor, UINT width, UINT height);

		DX12Resource* GetResource();

		~DX12RenderTarget();
	private:
		DX12Resource* resource;
		D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle;
		DX12Viewport* viewport;
	};
}