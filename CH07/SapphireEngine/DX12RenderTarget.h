#pragma once

#include <d3d12.h>
#include "Utils.h"

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
		/// Creates new Render Target instance and stores pointers to associated resource, descriptor handle and the initial resource state.
		/// </summary>
		/// <param name="device">DirectX 12 device used to create new Render Target</param>
		/// <param name="resource">A resource associated with the Render Target</param>
		/// <param name="descriptor">A Descriptor Handle to use during Render Target creation</param>
		/// <param name="state">An initial state of the resource</param>
		DX12RenderTarget(ID3D12Device* device, ID3D12Resource* resource, D3D12_CPU_DESCRIPTOR_HANDLE descriptor, D3D12_RESOURCE_STATES state);

		~DX12RenderTarget();
	private:
		ID3D12Resource* resource;
		D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle;
		D3D12_RESOURCE_STATES resourceState;
	};
}