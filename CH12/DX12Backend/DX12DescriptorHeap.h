#pragma once

#ifdef DX12BACKEND_EXPORTS
#define GPUAPI_DX12_API __declspec(dllexport)
#else
#define GPUAPI_DX12_API __declspec(dllimport)
#endif

#include <d3d12.h>

#include "Utils.h"

namespace Sapphire
{
	class DX12Device;

	/// <summary>
	/// A wrapper class for the ID3D12DescriptorHeap interface. It allocates the fixed amount of descriptors and then keeps the index of the first one available.
	/// </summary>
	class GPUAPI_DX12_API DX12DescriptorHeap
	{
		friend class DX12CommandList;
	public:
		/// <summary>
		/// Creates new Descriptor Heap instance of the given type.
		/// </summary>
		/// <param name="device">DirectX 12 device used to create new Descriptor Heap</param>
		/// <param name="type">A type of the new Descriptor Heap</param>
		DX12DescriptorHeap(DX12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type);

		~DX12DescriptorHeap();

		/// <summary>
		/// Returns the first available descriptor handle, then moves the internal index one position up.
		/// </summary>
		SIZE_T AllocateDescriptor();
		D3D12_GPU_DESCRIPTOR_HANDLE GetFirstGpuDescriptor();
		D3D12_CPU_DESCRIPTOR_HANDLE GetFirstCpuDescriptor();
		UINT GetDescriptorSize();
		UINT GetHeapSize();
	private:
		ID3D12DescriptorHeap* heap;
		UINT descriptorSize;
		UINT baseIndex;
		BOOL isShaderVisible;
	};
}