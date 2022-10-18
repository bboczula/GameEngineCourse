#include "Renderer.h"
#include "DX12VertexBuffer.h"
#include "DX12IndexBuffer.h"

Sapphire::Renderer::Renderer(HWND hwnd, LONG width, LONG height) : deviceContext(nullptr), renderContext(nullptr)
{
	Logger::GetInstance().Log("%s\n", "Sapphire::Renderer::Renderer()");

	deviceContext = new DeviceContext(hwnd, width, height);
	renderContext = new RenderContext(deviceContext, width, height);
}

Sapphire::Renderer::~Renderer()
{
	Logger::GetInstance().Log("%s\n", "Sapphire::Renderer::~Renderer()");

	delete renderContext;
	delete deviceContext;
}

void Sapphire::Renderer::Render(std::vector<GameObject*> objects)
{
	renderContext->Render(objects);
	renderContext->Execute(deviceContext);
	deviceContext->Present();
}

void Sapphire::Renderer::CreateResources(std::vector<GameObject*> objects)
{
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
	}

	// Create per-render-context resources
	renderContext->CreateResources(deviceContext, objects);
}

void Sapphire::Renderer::SetMainCamera(Camera* camera)
{
	renderContext->SetCamera(camera);
}
