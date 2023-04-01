#pragma once

#include <Windows.h>
#include <string>
#include "PixelDefinition.h"

namespace Sapphire
{
	class DX12Texture;
	class DX12VertexBuffer;
	class DX12IndexBuffer;

	class GameObject
	{
		friend class Renderer;
		friend class RenderInterface;
		friend class ForwardRenderingPass;
		friend class ShadowMapPass;
		friend class DeferredRenderingPass;
	public:
		GameObject();
		~GameObject();
		virtual void Initialize();
		virtual void Update(float deltaTime);
		void SetRotate(float x, float y, float z);
		void SetScale(float x, float y, float z);
		void SetPosition(float x, float y, float z);
		bool GetIsVisible();
		void SetIsVisible(bool isVisible);
		DirectX::SimpleMath::Vector3 GetTranslation();
		void SetTranslation(float x, float y, float z);
		DirectX::SimpleMath::Vector3 GetScale();
		DirectX::SimpleMath::Vector3 GetRotate();
		//void SetScale(float x, float y, float z);
		// START: Flexible Vertex Buffers (FVB) !--
		DirectX::SimpleMath::Vector4* position;
		DirectX::SimpleMath::Vector4* normal;
		DirectX::SimpleMath::Vector4* tangent;
		DirectX::SimpleMath::Vector2* albedoTexCoord;
		// END: Flexible Vertex Buffers (FVB) !--
		UINT numOfVertices;
		UINT* indices;
		UINT numOfIndices;
		PixelDefinition* pixels;
		UINT textureWidth;
		UINT textureHeight;
		PixelDefinition* bumpMapPixels;
		UINT bumpMapWidth;
		UINT bumpMapHeight;
		DirectX::SimpleMath::Matrix world;
		std::string name;
		void CalculateWorldMatrix();
	private:
		// START: Flexible Vertex Buffers (FVB) !--
		DX12VertexBuffer* positionVertexBuffer;
		DX12VertexBuffer* normalVertexBuffer;
		DX12VertexBuffer* tangentVertexBuffer;
		DX12VertexBuffer* colorTexCoordVertexBuffer;
		DX12IndexBuffer* indexBuffer;
		// END: Flexible Vertex Buffers (FVB) !--
		DX12Texture* texture;
		DX12Texture* bumpMap;
		DirectX::SimpleMath::Vector3 rotation;
		DirectX::SimpleMath::Vector3 scale;
		DirectX::SimpleMath::Vector3 translation;
		bool isVisible;
	};
}