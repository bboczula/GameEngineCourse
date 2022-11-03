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
	// Forward Declarations
	class DX12Device;
	class DX12CommandList;

	/// <summary>
	/// A wrapper class for the ID3D12CommandQueue interface. Contains the Fence  for synchronization.
	/// </summary>
	class GPUAPI_DX12_API DX12CommandQueue
	{
	public:

		/// <summary>
		/// Creates new CommandQeueue instance.
		/// </summary>
		/// <param name="device">DirectX 12 device used to create new Command Queue</param>
		DX12CommandQueue(DX12Device* device);

		~DX12CommandQueue();

		/// <summary>
		/// Returns a pointer for wrapped DirectX 12 Command Queue interface.
		/// </summary>
		/// <returns>A raw pointer to ID3D12CommandQueue</returns>
		ID3D12CommandQueue* Get();

		/// <summary>
		/// Executes provided command list and fulshes the GPU queue.
		/// </summary>
		/// <param name="commandList">Single command list to execute</param>
		void Execute(DX12CommandList* commandList);

		/// <summary>
		/// Increments the GPU fence and halts current CPU thread until this value is reached.
		/// </summary>
		void Flush();
	private:
		void CreateCommandQueue(DX12Device* device);
		void CreateFence(DX12Device* device);
		void CreateEventObject();
		void Signal();
		void WaitForGpu();
		ID3D12CommandQueue* commandQueue;
		HANDLE fenceEvent;
		UINT64 fenceValue;
		ID3D12Fence* fence;
	};
}