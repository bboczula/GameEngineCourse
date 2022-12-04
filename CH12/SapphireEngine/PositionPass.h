#pragma once

#include "RenderPass.h"

namespace Sapphire
{
	class RenderContex;
	class Camera;

	class PositionPass : public RenderPass
	{
	public:
		// You manually have to create Constructors
		PositionPass(RenderContext* renderContext, unsigned int width, unsigned int height);
		~PositionPass();
		void PreRender(DX12CommandList* commandList) override;
		void Render(DX12CommandList* commandList, RenderContext* renderContext, std::vector<GameObject*> objects) override;
		void PostRender(DX12CommandList* commandList) override;
		void SetCamera(Camera* camera);
	private:
		DX12Shader* pixelShader;
		DX12Shader* vertexShader;
		Camera* camera;
	};
}