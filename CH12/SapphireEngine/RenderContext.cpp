#include "RenderContext.h"
#include "../DX12Backend/DX12ConstantBuffer.h"
#include "../DX12Backend/DX12VertexBuffer.h"
#include "../DX12Backend/DX12IndexBuffer.h"
#include "../DX12Backend/DX12Texture.h"
#include "Light.h"

Sapphire::RenderContext::RenderContext(HWND hwnd, unsigned int width, unsigned int height) : deviceContext(deviceContext)
{
	Logger::GetInstance().Log("%s\n", "Sapphire::RenderContext::RenderContext");

	deviceContext = new DeviceContext(hwnd, width, height);

	// Create all the Descriptor Heaps
	rtvDescriptorHeap = deviceContext->CreateRtvDescriptorHeap();
	dsvDescriptorHeap = deviceContext->CreateDsvDescriptorHeap();
	srvDescriptorHeap = deviceContext->CreateSrvDescriptorHeap();

	// Each Render Context should have an active Render Target
	// Do I have to double buffer the Render Target? Probably not, since I do the lock-step rendering really
	// I need to update the DX12RenderTarget interface
	// renderTarget = new DX12RenderTarget()
	
	// D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;
	// rtvHandle.ptr = rtvDescriptorHeap->AllocateDescriptor();
	// renderTarget = new DX12RenderTarget(deviceContext->GetDevice(), rtvHandle, 1280, 720);
	//renderTarget = CreateRenderTarget(deviceContext);

	// Need to think if the the depth buffer is required for Render Context
	// D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle;
	// dsvHandle.ptr = dsvDescriptorHeap->AllocateDescriptor();
	// depthBuffer = new DX12DepthBuffer(deviceContext->GetDevice(), dsvHandle);
	//depthBuffer = CreateDepthBuffer(deviceContext);

	// Create the Command List
	commandList = deviceContext->CreateCommandList(); // new DX12CommandList(deviceContext->GetDevice());

	// Create required shaders
	//vertexShader = new DX12Shader("directional_texture_vs.cso");
	//pixelShader = new DX12Shader("directional_texture_ps.cso");

	// Create pipeline states and viewport
	//dxPipelineState = new DX12PipelineState(deviceContext->GetDevice(), vertexShader, pixelShader);
	//viewport = new DX12Viewport(1280, 720);

	// Create main camera
	//camera = new Camera(1280.0f / 720.0f);

	// Create global directional light
	directionalLight = new Light(0.0f, 1.0f, 0.0f);

	// Create Shadow Map Pass
	shadowMapPass = new ShadowMapPass(this, directionalLight);

	// Create Render Pass
	renderPass = new ForwardRenderingPass(this, directionalLight, width, height);
}

Sapphire::RenderContext::~RenderContext()
{
	Logger::GetInstance().Log("%s\n", "Sapphire::RenderContext::~RenderContext");

	delete shadowMapPass;
	delete renderPass;
	//delete viewport;
	//delete dxPipelineState;
	delete commandList;
	//delete depthBuffer;
	//delete renderTarget;
	delete srvDescriptorHeap;
	delete dsvDescriptorHeap;
	delete rtvDescriptorHeap;
}

void Sapphire::RenderContext::Setup()
{
	//const float clearColor[] = { 0.1176f, 0.1882f, 0.4470f, 1.0f };
	////unsigned int currentFrameIndex = deviceContext->GetCurrentFrameIndex();
	//
	//commandList->Reset();
	//commandList->TransitionTo(renderTarget->GetResource(), D3D12_RESOURCE_STATE_RENDER_TARGET);
	//commandList->SetPipelineState(dxPipelineState);
	//commandList->SetRenderTarget(renderTarget, depthBuffer);
	//commandList->ClearRenderTarget(renderTarget, clearColor);
	//// Do I have to manage resource states?
	//commandList->ClearDepthBuffer(depthBuffer);
	//commandList->SetConstantBuffer(0, 16, camera->GetViewMatrixPtr());
	//commandList->SetConstantBuffer(1, 16, camera->GetProjectionMatrixPtr());
	// For texture development
	// THIS IS RENDER CONTEXT
	renderPass->Setup(commandList);
	commandList->SetDescriptorHeap(srvDescriptorHeap);
}

void Sapphire::RenderContext::CreateResources(std::vector<GameObject*> objects)
{
	commandList->Reset();

	for (int i = 0; i < objects.size(); i++)
	{
		// START: Flexible Vertex Buffers (FVB) !--
		// TODO: needs to decouple Index Buffer
		if (objects[i]->numOfVertices != 0)
		{
			objects[i]->positionVertexBuffer = new DX12VertexBuffer(deviceContext->GetDevice(), objects[i]->position,
				sizeof(DirectX::SimpleMath::Vector4), objects[i]->numOfVertices);
		}
		if (objects[i]->numOfVertices != 0)
		{
			objects[i]->normalVertexBuffer = new DX12VertexBuffer(deviceContext->GetDevice(), objects[i]->normal,
				sizeof(DirectX::SimpleMath::Vector4), objects[i]->numOfVertices);
		}
		if (objects[i]->numOfVertices != 0)
		{
			objects[i]->tangentVertexBuffer = new DX12VertexBuffer(deviceContext->GetDevice(), objects[i]->tangent,
				sizeof(DirectX::SimpleMath::Vector4), objects[i]->numOfVertices);
		}
		if (objects[i]->numOfVertices != 0)
		{
			objects[i]->colorTexCoordVertexBuffer = new DX12VertexBuffer(deviceContext->GetDevice(), objects[i]->albedoTexCoord,
				sizeof(DirectX::SimpleMath::Vector2), objects[i]->numOfVertices);
		}
		if (objects[i]->numOfIndices != 0)
		{
			objects[i]->indexBuffer = new DX12IndexBuffer(deviceContext->GetDevice(), objects[i]->indices,
				sizeof(UINT), objects[i]->numOfIndices);
		}
		// END: Flexible Vertex Buffers (FVB) !--

		// Some objects are not renderable, need to think of a better way to handle this
		if (!objects[i]->numOfVertices)
		{
			continue;
		}

		CreateTextureResource(objects[i]->texture, objects[i]->textureWidth, objects[i]->textureHeight, objects[i]->pixels);

		// If Game Object has some pixels in the bumpMap
		if (objects[i]->bumpMapPixels != nullptr)
		{
			CreateTextureResource(objects[i]->bumpMap, objects[i]->bumpMapWidth, objects[i]->bumpMapHeight, objects[i]->bumpMapPixels);
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

void Sapphire::RenderContext::CreateTextureResource(DX12Texture*& dest, UINT width, UINT height, PixelDefinition* source)
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

Sapphire::DX12RenderTarget* Sapphire::RenderContext::CreateRenderTarget(UINT width, UINT height)
{
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;
	rtvHandle.ptr = rtvDescriptorHeap->AllocateDescriptor();
	return new DX12RenderTarget(deviceContext->GetDevice(), rtvHandle, width, height);
}

Sapphire::DX12DepthBuffer* Sapphire::RenderContext::CreateDepthBuffer(UINT width, UINT height)
{
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle;
	dsvHandle.ptr = dsvDescriptorHeap->AllocateDescriptor();
	return new DX12DepthBuffer(deviceContext->GetDevice(), dsvHandle, width, height);
}

Sapphire::DX12DepthBuffer* Sapphire::RenderContext::CreateDepthBufferWithSrv(UINT width, UINT height)
{
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle;
	dsvHandle.ptr = dsvDescriptorHeap->AllocateDescriptor();

	auto descriptorIndex = srvDescriptorHeap->GetHeapSize();

	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle;
	srvHandle.ptr = srvDescriptorHeap->AllocateDescriptor();

	return new DX12DepthBuffer(deviceContext->GetDevice(), dsvHandle, width, height, srvHandle, descriptorIndex);
}

Sapphire::DX12ConstantBuffer* Sapphire::RenderContext::CreateConstantBuffer()
{
	auto descriptorIndex = srvDescriptorHeap->GetHeapSize();
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle;
	srvHandle.ptr = srvDescriptorHeap->AllocateDescriptor();

	return new DX12ConstantBuffer(deviceContext->GetDevice(), 256, srvHandle, descriptorIndex);
}

Sapphire::DX12PipelineState* Sapphire::RenderContext::CreatePipelineState(DX12Shader* vertexShader, DX12Shader* pixelShader, DX12InputLayout* inputLayout)
{
	return new DX12PipelineState(deviceContext->GetDevice(), vertexShader, pixelShader, inputLayout, false);
}

D3D12_GPU_DESCRIPTOR_HANDLE Sapphire::RenderContext::GetSrvDescriptor(UINT32 index)
{
	D3D12_GPU_DESCRIPTOR_HANDLE descriptor;
	descriptor.ptr = srvDescriptorHeap->GetFirstGpuDescriptor().ptr + index * srvDescriptorHeap->GetDescriptorSize();
	return descriptor;
}

void Sapphire::RenderContext::Render(std::vector<GameObject*> objects)
{
	if (directionalLight->GetPositionY() > 0)
	{
		directionalLight->RotateX(0.025f);
		//Logger::GetInstance().Log("Directional Light: %f %f %f\n", directionalLight->GetPositionX(), directionalLight->GetPositionY(), directionalLight->GetPositionZ());
	}

	shadowMapPass->Setup(commandList);
	commandList->SetDescriptorHeap(srvDescriptorHeap);
	shadowMapPass->Render(commandList, this, objects);
	shadowMapPass->Teardown(commandList);

	renderPass->Setup(commandList);
	renderPass->Render(commandList, this, objects, shadowMapPass->GetDepthBuffer(), shadowMapPass->camera);
	renderPass->Teardown(commandList);


	unsigned int currentFrameIndex = deviceContext->GetCurrentFrameIndex();
	//blitPass->Render(commandList, this, GetRenderTarget()->GetResource(), dxResources[currentFrameIndex]);
	Blit(GetRenderTarget()->GetResource(), dxResources[currentFrameIndex]);

	commandList->Close();
}

void Sapphire::RenderContext::Teardown()
{
	//commandList->TransitionTo(renderTarget->GetResource(), D3D12_RESOURCE_STATE_COPY_SOURCE);
	renderPass->Teardown(commandList);
	commandList->Close();
}

void Sapphire::RenderContext::Execute()
{
	deviceContext->Execute(commandList);
	deviceContext->Present();
}

Sapphire::DX12RenderTarget* Sapphire::RenderContext::GetRenderTarget()
{
	//return renderTarget;
	return renderPass->GetRenderTarget();
}

void Sapphire::RenderContext::SetCamera(Camera* camera)
{
	//this->camera = camera;
	renderPass->SetCamera(camera);
}

void Sapphire::RenderContext::Blit(DX12Resource* source, DX12Resource* destination)
{
	commandList->TransitionTo(source, D3D12_RESOURCE_STATE_COPY_SOURCE);
	commandList->TransitionTo(destination, D3D12_RESOURCE_STATE_COPY_DEST);
	commandList->CopyTextureRegion(source, destination);
	commandList->TransitionTo(destination, D3D12_RESOURCE_STATE_PRESENT);
}
