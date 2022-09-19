#pragma once

#include <d3d12.h>

#include "Logger.h"
#include "Utils.h"
#include "DX12RenderTarget.h"
#include "DX12DepthBuffer.h"
#include "DX12PipelineState.h"
#include "DX12Device.h"
#include "DX12Viewport.h"
#include "DX12DescriptorHeap.h"
#include "DX12Texture.h"
#include "PixelDefinition.h"

namespace Sapphire
{
	// Forward declarations
	class DX12VertexBuffer;
	class DX12IndexBuffer;

	/// <summary>
	/// A wrapper class for the ID3D12CommandList interface. Contains the ID3D12CommandAllocater too.
	/// </summary>
	class DX12CommandList
	{
		friend class DX12CommandQueue;
	public:
		/// <summary>
		/// Creates new Command List, along with the correspoinding Command Allocator.
		/// </summary>
		/// <param name="device">DirectX 12 device used to create new Command List</param>
		DX12CommandList(DX12Device* device);

		~DX12CommandList();

		// THIS IS REALLY BAD, I DON'T WANT TO EXPOSE THIS
		ID3D12GraphicsCommandList* GetCommandList();

		/// <summary>
		/// Transitions a Resource from its current state to the new state.
		/// </summary>
		/// <param name="resource">A Resource we want to transition</param>
		/// <param name="nextState">A stat to which we want to transition to</param>
		void TransitionTo(DX12Resource* resource, D3D12_RESOURCE_STATES nextState);

		/// <summary>
		/// Sets given Render Target as the current Render Target.
		/// </summary>
		/// <param name="renderTarget">A Render Target we want as the new Render Target</param>
		void SetRenderTarget(DX12RenderTarget* renderTarget);

		void SetRenderTarget(DX12RenderTarget* renderTarget, DX12DepthBuffer* depthBuffer);

		void SetDescriptorHeap(DX12DescriptorHeap* descriptorHeap);

		/// <summary>
		/// Clears given Render Target with given color.
		/// </summary>
		/// <param name="renderTarget">A Render Target we want to clear</param>
		/// <param name="color">A color of the clear operation</param>
		void ClearRenderTarget(DX12RenderTarget* renderTarget, const float* color);

		void ClearDepthBuffer(DX12DepthBuffer* depthBuffer);

		void SetConstantBuffer(UINT slot, UINT numOf32BitValues, void* data);

		void SetTexture(UINT slot, D3D12_GPU_DESCRIPTOR_HANDLE textureDescriptorHandle);

		void SetConstantBuffer(UINT slot, D3D12_GPU_DESCRIPTOR_HANDLE textureDescriptorHandle);

		void CopyTextureRegion(DX12Resource* source, DX12Resource* destination);

		void CopyTextureRegion(DX12Resource* source, DX12Texture* destination, UINT width, UINT height);

		/// <summary>
		/// Close the Command List.
		/// </summary>
		void Close();

		/// <summary>
		/// Reset the Command List and Command Allocator. Make sure that the GPU is done processing it.
		/// </summary>
		void Reset();

		// CH08
		void SetPipelineState(DX12PipelineState* pipelineState);

		void Draw(DX12VertexBuffer* vertexBuffer, DX12IndexBuffer* indexBuffer);

		void Draw(DX12VertexBuffer* positionVertexBuffer, DX12VertexBuffer* normalVertexBuffer, DX12IndexBuffer* indexBuffer);

		void Draw(DX12VertexBuffer* positionVertexBuffer, DX12VertexBuffer* normalVertexBuffer, DX12VertexBuffer* albedoTexCoordVertexBuffer, DX12IndexBuffer* indexBuffer);

		void Draw(DX12VertexBuffer* positionVertexBuffer, DX12VertexBuffer* normalVertexBuffer, DX12VertexBuffer* tangentVertexBuffer, DX12VertexBuffer* albedoTexCoordVertexBuffer, DX12IndexBuffer* indexBuffer);

	private:
		ID3D12CommandAllocator* commandAllocator;
		ID3D12GraphicsCommandList* commandList;
	};
}