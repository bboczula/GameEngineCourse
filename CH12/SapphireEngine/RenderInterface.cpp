#include "RenderInterface.h"
#include "../DX12Backend/DX12ConstantBuffer.h"
#include "../DX12Backend/DX12VertexBuffer.h"
#include "../DX12Backend/DX12IndexBuffer.h"
#include "../DX12Backend/DX12Texture.h"
#include "DeferredRenderingPass.h"
#include "LightResolvePass.h"
#include "GameObjectTree.h"

Sapphire::RenderInterface::RenderInterface(HWND hwnd, unsigned int width, unsigned int height)
	: deviceContext(deviceContext), dxResources{ nullptr }, renderTargets{ nullptr }, uploadBuffer{ nullptr }, hwnd{ hwnd }
{
	Logger::GetInstance().Log("%s\n", "Sapphire::RenderInterface::RenderInterface");

	deviceContext = new DeviceContext(hwnd, width, height);

	// Create all the Descriptor Heaps
	rtvDescriptorHeap = deviceContext->CreateRtvDescriptorHeap();
	dsvDescriptorHeap = deviceContext->CreateDsvDescriptorHeap();
	srvDescriptorHeap = deviceContext->CreateSrvDescriptorHeap();

	// Create the Command List
	commandList = deviceContext->CreateCommandList();
}

Sapphire::RenderInterface::~RenderInterface()
{
	Logger::GetInstance().Log("%s\n", "Sapphire::RenderInterface::~RenderInterface");

	delete commandList;
	delete srvDescriptorHeap;
	delete dsvDescriptorHeap;
	delete rtvDescriptorHeap;
}

void Sapphire::RenderInterface::CreateResources(GameObjectTree* gameObjectTree)
{
	commandList->Reset();

	for (int i = 0; i < gameObjectTree->Size(); i++)
	{
		// START: Flexible Vertex Buffers (FVB) !--
		// TODO: needs to decouple Index Buffer
		if (gameObjectTree->At(i)->numOfVertices != 0)
		{
			gameObjectTree->At(i)->positionVertexBuffer = new DX12VertexBuffer(deviceContext->GetDevice(), gameObjectTree->At(i)->position,
				sizeof(DirectX::SimpleMath::Vector4), gameObjectTree->At(i)->numOfVertices);
		}
		if (gameObjectTree->At(i)->numOfVertices != 0)
		{
			gameObjectTree->At(i)->normalVertexBuffer = new DX12VertexBuffer(deviceContext->GetDevice(), gameObjectTree->At(i)->normal,
				sizeof(DirectX::SimpleMath::Vector4), gameObjectTree->At(i)->numOfVertices);
		}
		if (gameObjectTree->At(i)->numOfVertices != 0)
		{
			gameObjectTree->At(i)->tangentVertexBuffer = new DX12VertexBuffer(deviceContext->GetDevice(), gameObjectTree->At(i)->tangent,
				sizeof(DirectX::SimpleMath::Vector4), gameObjectTree->At(i)->numOfVertices);
		}
		if (gameObjectTree->At(i)->numOfVertices != 0)
		{
			gameObjectTree->At(i)->colorTexCoordVertexBuffer = new DX12VertexBuffer(deviceContext->GetDevice(), gameObjectTree->At(i)->albedoTexCoord,
				sizeof(DirectX::SimpleMath::Vector2), gameObjectTree->At(i)->numOfVertices);
		}
		if (gameObjectTree->At(i)->numOfIndices != 0)
		{
			gameObjectTree->At(i)->indexBuffer = new DX12IndexBuffer(deviceContext->GetDevice(), gameObjectTree->At(i)->indices,
				sizeof(UINT), gameObjectTree->At(i)->numOfIndices);
		}
		// END: Flexible Vertex Buffers (FVB) !--

		// Some objects are not renderable, need to think of a better way to handle this
		if (!gameObjectTree->At(i)->numOfVertices)
		{
			continue;
		}

		CreateTextureResource(gameObjectTree->At(i)->texture, gameObjectTree->At(i)->textureWidth, gameObjectTree->At(i)->textureHeight, gameObjectTree->At(i)->pixels);

		// If Game Object has some pixels in the bumpMap
		if (gameObjectTree->At(i)->bumpMapPixels != nullptr)
		{
			CreateTextureResource(gameObjectTree->At(i)->bumpMap, gameObjectTree->At(i)->bumpMapWidth, gameObjectTree->At(i)->bumpMapHeight, gameObjectTree->At(i)->bumpMapPixels);
		}
	}

	// You would have to create the texture, but you don't really have to upload it.
	// But then what, you would have to create DsvDescriptor somwhere else, which is not flexible at all

	// This is not optimal at all, but should get the job done for now.
	commandList->Close();
	deviceContext->Execute(commandList);

	// Create resources shared between Render Passes

	// Create resources for the back buffer
	for (UINT i = 0; i < FRAME_COUNT; i++)
	{
		dxResources[i] = deviceContext->GetBackBuffer(i);

		// Maybe this temporary thing could be avoided if i return the entire handle?
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;
		rtvHandle.ptr = rtvDescriptorHeap->AllocateDescriptor();
		renderTargets[i] = new DX12RenderTarget(deviceContext->GetDevice(), dxResources[i], rtvHandle);
	}
}

void Sapphire::RenderInterface::CreateTextureResource(DX12Texture*& dest, UINT width, UINT height, PixelDefinition* source)
{
	auto descriptorIndex = srvDescriptorHeap->GetHeapSize();
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle;
	srvHandle.ptr = srvDescriptorHeap->AllocateDescriptor();
	dest = new DX12Texture(deviceContext->GetDevice(), srvHandle, width, height, descriptorIndex);

	// You can create the Upload buffer of the required size
	const UINT64 uploadBufferSize = GetRequiredIntermediateSize(dest->GetResource()->GetResource(), 0, 1);
	uploadBuffer = DX12Resource::CreateUpload(deviceContext->GetDevice(), uploadBufferSize);

	Logger::GetInstance().Log("Create Upload size: %d, copy size: %d\n", uploadBufferSize, width * height * sizeof(PixelDefinition));

	// Then upload the raw data, using Map/Unmap method
	//uploadBuffer->Upload(objects[i]->pixels, objects[i]->textureWidth * objects[i]->textureHeight * sizeof(PixelDefinition));
	uploadBuffer->Upload(&source[0].color.x, width, height, sizeof(PixelDefinition), Align(width * sizeof(PixelDefinition), D3D12_TEXTURE_DATA_PITCH_ALIGNMENT));

	// Finally, you can copy from Upload buffer to Texture buffer using CopyTextureRegion
	D3D12_SUBRESOURCE_DATA textureData = {};
	textureData.pData = &source;
	textureData.RowPitch = width * sizeof(PixelDefinition);
	textureData.SlicePitch = textureData.RowPitch * height;

	// This is a Command on a Command List, so I need to execute it probably or something;
	commandList->CopyTextureRegion(uploadBuffer, dest, width, height);

	// Here you also should transition from COPY_DEST to PIXEL_SHADER_VISIBLE
	commandList->TransitionTo(dest->GetResource(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
}

Sapphire::DX12RenderTarget* Sapphire::RenderInterface::CreateRenderTarget(RenderTargetNames name, UINT width, UINT height, DX12RenderTarget::Format format)
{
	// Store GPU Descriptor Handle for easy usage
	auto heapSize = rtvDescriptorHeap->GetHeapSize();
	auto descriptorSize = rtvDescriptorHeap->GetDescriptorSize();
	renderTargetList[name] = rtvDescriptorHeap->GetFirstCpuDescriptor().ptr + INT64(heapSize) * INT64(descriptorSize);

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;
	rtvHandle.ptr = rtvDescriptorHeap->AllocateDescriptor();
	return new DX12RenderTarget(deviceContext->GetDevice(), rtvHandle, width, height, format);
}

Sapphire::DX12DepthBuffer* Sapphire::RenderInterface::CreateDepthBuffer(UINT width, UINT height)
{
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle;
	dsvHandle.ptr = dsvDescriptorHeap->AllocateDescriptor();
	return new DX12DepthBuffer(deviceContext->GetDevice(), dsvHandle, width, height);
}

Sapphire::DX12DepthBuffer* Sapphire::RenderInterface::CreateDepthBufferWithSrv(RenderTargetNames name, UINT width, UINT height)
{
	// Store GPU Descriptor Handle for easy usage
	auto heapSize = srvDescriptorHeap->GetHeapSize();
	auto descriptorSize = srvDescriptorHeap->GetDescriptorSize();
	renderTargetList[name] = srvDescriptorHeap->GetFirstGpuDescriptor().ptr + INT64(heapSize) * INT64(descriptorSize);

	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle;
	dsvHandle.ptr = dsvDescriptorHeap->AllocateDescriptor();

	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle;
	srvHandle.ptr = srvDescriptorHeap->AllocateDescriptor();

	return new DX12DepthBuffer(deviceContext->GetDevice(), dsvHandle, width, height, srvHandle, heapSize);
}

Sapphire::DX12ConstantBuffer* Sapphire::RenderInterface::CreateConstantBuffer()
{
	auto descriptorIndex = srvDescriptorHeap->GetHeapSize();
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle;
	srvHandle.ptr = srvDescriptorHeap->AllocateDescriptor();

	return new DX12ConstantBuffer(deviceContext->GetDevice(), 256, srvHandle, descriptorIndex);
}

Sapphire::DX12PipelineState* Sapphire::RenderInterface::CreatePipelineState(DX12Shader* vertexShader, DX12Shader* pixelShader, DX12InputLayout* inputLayout)
{
	return new DX12PipelineState(deviceContext->GetDevice());
}

D3D12_GPU_DESCRIPTOR_HANDLE Sapphire::RenderInterface::GetSrvDescriptor(UINT32 index)
{
	D3D12_GPU_DESCRIPTOR_HANDLE descriptor;
	descriptor.ptr = srvDescriptorHeap->GetFirstGpuDescriptor().ptr + index * srvDescriptorHeap->GetDescriptorSize();
	return descriptor;
}

D3D12_GPU_DESCRIPTOR_HANDLE Sapphire::RenderInterface::GetSrvDescriptor(RenderTargetNames name)
{
	D3D12_GPU_DESCRIPTOR_HANDLE output;
	output.ptr = renderTargetList[name];
	return output;
}

Sapphire::DX12DescriptorHeap* Sapphire::RenderInterface::GetSrvDescriptorHeap()
{
	return srvDescriptorHeap;
}

void Sapphire::RenderInterface::Blit(DX12Resource* input)
{
	unsigned int currentFrameIndex = deviceContext->GetCurrentFrameIndex();
	Blit(input, dxResources[currentFrameIndex]);
}

void Sapphire::RenderInterface::Execute()
{
	deviceContext->Execute(commandList);
	deviceContext->Present();
}

void Sapphire::RenderInterface::Blit(DX12Resource* source, DX12Resource* destination)
{
	commandList->TransitionTo(source, D3D12_RESOURCE_STATE_COPY_SOURCE);
	commandList->TransitionTo(destination, D3D12_RESOURCE_STATE_COPY_DEST);
	commandList->CopyTextureRegion(source, destination);
	commandList->TransitionTo(destination, D3D12_RESOURCE_STATE_PRESENT);
}

Sapphire::DX12Device* Sapphire::RenderInterface::GetDevice()
{
	return deviceContext->GetDevice();
}

Sapphire::DX12CommandList* Sapphire::RenderInterface::GetCommandList()
{
	return commandList;
}

void Sapphire::RenderInterface::SetSrvDescriptorHeap()
{
	commandList->SetDescriptorHeap(srvDescriptorHeap);
}

HWND Sapphire::RenderInterface::GetHwnd()
{
	return hwnd;
}
