#pragma once

#include "../DX12Backend/DX12RenderTarget.h"
#include "../DX12Backend/DX12ConstantBuffer.h"
#include "../DX12Backend/DX12DepthBuffer.h"
#include "../DX12Backend/DX12Shader.h"
#include "../DX12Backend/DX12PipelineState.h"
#include "../DX12Backend/DX12Viewport.h"
#include "RenderPass.h"

namespace Sapphire
{
	class RenderInterface;
	class DX12InputLayout;
	class DX12ConstantBuffer;
	class Camera;

	class ForwardRenderingPass : public Sapphire::RenderPass
	{
	public:
		ForwardRenderingPass(RenderInterface* renderInterface, unsigned int width, unsigned int height);
		~ForwardRenderingPass();
		void PreRender(DX12CommandList* commandList) override;
		void Render(DX12CommandList* commandList, RenderInterface* renderInterface, std::vector<GameObject*> objects, std::vector<LightObject*> lights) override;
		void Render(DX12CommandList* commandList, RenderInterface* renderInterface, std::vector<GameObject*> objects, Camera* shadowMapCamera);
		void PostRender(DX12CommandList* commandList) override;
		void SetCamera(Camera* camera);
		void SetShadowCamera(Camera* camera);
	private:
		DX12ConstantBuffer* constantBuffer;
		DX12Shader* pixelShader;
		DX12Shader* vertexShader;
		DX12Shader* pixelShader_noBump;
		DX12Shader* vertexShader_noBump;
		Camera* camera;
		Camera* shadowCamera;
	};
}