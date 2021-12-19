#pragma once

#include "Utils.h"
#include "Logger.h"

#include <d3d12.h>

namespace Sapphire
{
	/// <summary>
	/// This class represents DirectX 12 resources. It also keeps track of the internal reasource state.
	/// </summary>
	class DX12Resource
	{
		friend class DX12CommandList;
	public:
		/// <summary>
		/// Creates DirectX 12 resource, based on existing resource created externally.
		/// </summary>
		/// <param name="source">Pointer to a resource created externally</param>
		/// <param name="state">Initial state of the resource</param>
		DX12Resource(void* source, D3D12_RESOURCE_STATES state);

		~DX12Resource();
		/// <summary>
		/// Returns raw pointer to the resource.
		/// </summary>
		ID3D12Resource* GetResource();
	private:
		D3D12_RESOURCE_STATES state;
		ID3D12Resource* resource;
	};
}