#pragma once

#include <d3d12.h>

#include "Logger.h"
#include "Utils.h"

namespace Sapphire
{
	/// <summary>
	/// A wrapper class for the ID3D12DescriptorHeap interface. It allocates the fixed amount of descriptors and then keeps the index of the first one available.
	/// </summary>
	class DX12DescriptorHeap
	{
	public:
		/// <summary>
		/// Creates new Descriptor Heap instance of the given type.
		/// </summary>
		/// <param name="device">DirectX 12 device used to create new Descriptor Heap</param>
		/// <param name="type">A type of the new Descriptor Heap</param>
		DX12DescriptorHeap(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type);

		~DX12DescriptorHeap();

		/// <summary>
		/// Returns the first available descriptor handle, then moves the internal index one position up.
		/// </summary>
		SIZE_T AllocateDescriptor();
	private:
		ID3D12DescriptorHeap* heap;
		UINT descriptorSize;
		UINT baseIndex;
	};
}