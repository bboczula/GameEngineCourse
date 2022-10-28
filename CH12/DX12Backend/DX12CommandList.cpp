#include "pch.h"

#include "DX12CommandList.h"
#include "DX12VertexBuffer.h"
#include "DX12IndexBuffer.h"

GpuApi::DX12CommandList::DX12CommandList(DX12Device* device)
{
	ExitIfFailed(device->GetDevice()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator)));
	ExitIfFailed(device->GetDevice()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator, nullptr, IID_PPV_ARGS(&commandList)));
	commandList->Close();
}

GpuApi::DX12CommandList::~DX12CommandList()
{
	SafeRelease(&commandList);
	SafeRelease(&commandAllocator);
}

ID3D12GraphicsCommandList* GpuApi::DX12CommandList::GetCommandList()
{
	return commandList;
}

void GpuApi::DX12CommandList::TransitionTo(DX12Resource* resource, D3D12_RESOURCE_STATES nextState)
{
	D3D12_RESOURCE_BARRIER barrier;
	ZeroMemory(&barrier, sizeof(barrier));
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = resource->resource;
	barrier.Transition.StateBefore = resource->state;
	barrier.Transition.StateAfter = nextState;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	commandList->ResourceBarrier(1, &barrier);

	resource->state = nextState;
}

void GpuApi::DX12CommandList::SetRenderTarget(DX12RenderTarget* renderTarget)
{
	commandList->OMSetRenderTargets(1, &renderTarget->descriptorHandle, FALSE, nullptr);
	commandList->RSSetViewports(1, &renderTarget->viewport->viewport);
	commandList->RSSetScissorRects(1, &renderTarget->viewport->scissors);
}

void GpuApi::DX12CommandList::SetRenderTarget(DX12RenderTarget* renderTarget, DX12DepthBuffer* depthBuffer)
{
	commandList->OMSetRenderTargets(1, &renderTarget->descriptorHandle, FALSE, &depthBuffer->descriptorHandle);
	commandList->RSSetViewports(1, &renderTarget->viewport->viewport);
	commandList->RSSetScissorRects(1, &renderTarget->viewport->scissors);
}

void GpuApi::DX12CommandList::SetDescriptorHeap(DX12DescriptorHeap* descriptorHeap)
{
	commandList->SetDescriptorHeaps(1, &descriptorHeap->heap);
}

void GpuApi::DX12CommandList::SetPipelineState(DX12PipelineState* pipelineState)
{
	commandList->SetGraphicsRootSignature(pipelineState->rootSignature);
	commandList->SetPipelineState(pipelineState->pipelineState);
}

void GpuApi::DX12CommandList::Draw(DX12VertexBuffer* positionVertexBuffer, DX12IndexBuffer* indexBuffer)
{
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	commandList->IASetVertexBuffers(0, 1, &positionVertexBuffer->vertexBufferView);
	// CH12 - Added Index Buffer handling; this could be also part of CH11
	commandList->IASetIndexBuffer(&indexBuffer->indexBufferView);
	commandList->DrawIndexedInstanced(indexBuffer->GetNumOfIndices(), 1, 0, 0, 0);
}

void GpuApi::DX12CommandList::Draw(DX12VertexBuffer* positionVertexBuffer, DX12VertexBuffer* normalVertexBuffer, DX12IndexBuffer* indexBuffer)
{
	D3D12_VERTEX_BUFFER_VIEW vertexBufferViews[] = { positionVertexBuffer->vertexBufferView, normalVertexBuffer->vertexBufferView };
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	commandList->IASetVertexBuffers(0, 2, vertexBufferViews);
	// CH12 - Added Index Buffer handling; this could be also part of CH11
	commandList->IASetIndexBuffer(&indexBuffer->indexBufferView);
	commandList->DrawIndexedInstanced(indexBuffer->GetNumOfIndices(), 1, 0, 0, 0);
}

void GpuApi::DX12CommandList::Draw(DX12VertexBuffer* positionVertexBuffer, DX12VertexBuffer* normalVertexBuffer, DX12VertexBuffer* albedoTexCoordVertexBuffer, DX12IndexBuffer* indexBuffer)
{
	D3D12_VERTEX_BUFFER_VIEW vertexBufferViews[] =
	{ positionVertexBuffer->vertexBufferView, normalVertexBuffer->vertexBufferView, albedoTexCoordVertexBuffer->vertexBufferView };
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	commandList->IASetVertexBuffers(0, 3, vertexBufferViews);
	// CH12 - Added Index Buffer handling; this could be also part of CH11
	commandList->IASetIndexBuffer(&indexBuffer->indexBufferView);
	commandList->DrawIndexedInstanced(indexBuffer->GetNumOfIndices(), 1, 0, 0, 0);
}

void GpuApi::DX12CommandList::Draw(DX12VertexBuffer* positionVertexBuffer, DX12VertexBuffer* normalVertexBuffer, DX12VertexBuffer* tangentVertexBuffer, DX12VertexBuffer* albedoTexCoordVertexBuffer, DX12IndexBuffer* indexBuffer)
{
	D3D12_VERTEX_BUFFER_VIEW vertexBufferViews[] =
	{ positionVertexBuffer->vertexBufferView, normalVertexBuffer->vertexBufferView, tangentVertexBuffer->vertexBufferView, albedoTexCoordVertexBuffer->vertexBufferView };
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	commandList->IASetVertexBuffers(0, 4, vertexBufferViews);
	// CH12 - Added Index Buffer handling; this could be also part of CH11
	commandList->IASetIndexBuffer(&indexBuffer->indexBufferView);
	commandList->DrawIndexedInstanced(indexBuffer->GetNumOfIndices(), 1, 0, 0, 0);
}

void GpuApi::DX12CommandList::ClearRenderTarget(DX12RenderTarget* renderTarget, const float* color)
{
	commandList->ClearRenderTargetView(renderTarget->descriptorHandle, color, 0, nullptr);
}

void GpuApi::DX12CommandList::ClearDepthBuffer(DX12DepthBuffer* depthBuffer)
{
	commandList->ClearDepthStencilView(depthBuffer->descriptorHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, NULL);
}

void GpuApi::DX12CommandList::SetConstantBuffer(UINT slot, UINT numOf32BitValues, void* data)
{
	commandList->SetGraphicsRoot32BitConstants(slot, numOf32BitValues, data, 0);
}

void GpuApi::DX12CommandList::SetTexture(UINT slot, D3D12_GPU_DESCRIPTOR_HANDLE textureDescriptorHandle)
{
	commandList->SetGraphicsRootDescriptorTable(slot, textureDescriptorHandle);
}

void GpuApi::DX12CommandList::SetConstantBuffer(UINT slot, D3D12_GPU_DESCRIPTOR_HANDLE textureDescriptorHandle)
{
	commandList->SetGraphicsRootDescriptorTable(slot, textureDescriptorHandle);
}

void GpuApi::DX12CommandList::Close()
{
	ExitIfFailed(commandList->Close());
}

void GpuApi::DX12CommandList::CopyTextureRegion(DX12Resource* source, DX12Resource* destination)
{
	// Here you should copy the Render Target from the previous Render Context and copy it to the Back Buffer
	// commandList->CopyTextureRegion();
	D3D12_TEXTURE_COPY_LOCATION destLocation = {};
	destLocation.pResource = destination->GetResource();
	destLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	destLocation.SubresourceIndex = 0; // Not sure if this is correct;

	D3D12_TEXTURE_COPY_LOCATION srcLocation = {};
	srcLocation.pResource = source->GetResource();
	srcLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	srcLocation.SubresourceIndex = 0;
	commandList->CopyTextureRegion(&destLocation, 0, 0, 0, &srcLocation, nullptr);
}

void GpuApi::DX12CommandList::CopyTextureRegion(DX12Resource* source, DX12Texture* destination, UINT width, UINT height)
{
	// Here you should copy the Render Target from the previous Render Context and copy it to the Back Buffer
	// commandList->CopyTextureRegion();
	D3D12_TEXTURE_COPY_LOCATION destLocation = {};
	destLocation.pResource = destination->GetResource()->GetResource();
	destLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	destLocation.SubresourceIndex = 0; // Not sure if this is correct;

	D3D12_SUBRESOURCE_FOOTPRINT pitchedDesc = {};
	pitchedDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	pitchedDesc.Width = width;
	pitchedDesc.Height = height;
	pitchedDesc.Depth = 1;
	pitchedDesc.RowPitch = Align(width * sizeof(float) * 4, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT);

	D3D12_PLACED_SUBRESOURCE_FOOTPRINT placedTexture = {};
	placedTexture.Offset = 0;
	placedTexture.Footprint = pitchedDesc;

	D3D12_TEXTURE_COPY_LOCATION srcLocation = {};
	srcLocation.pResource = source->GetResource();
	srcLocation.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
	srcLocation.PlacedFootprint = placedTexture;
	commandList->CopyTextureRegion(&destLocation, 0, 0, 0, &srcLocation, nullptr);
}

void GpuApi::DX12CommandList::Reset()
{
	ExitIfFailed(commandAllocator->Reset());
	ExitIfFailed(commandList->Reset(commandAllocator, nullptr));
}
