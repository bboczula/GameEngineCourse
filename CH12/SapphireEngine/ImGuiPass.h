#pragma once

#include "RenderPass.h"

namespace Sapphire
{
	class RenderInterface;
	class DX12Shader;
	class GameObject;
	class GameObjectTree;

	class ImGuiPass : public RenderPass
	{
	public:
		ImGuiPass(RenderInterface* renderInterface, unsigned int width, unsigned int height, DX12MultiRenderTarget* multiRenderTarget);
		~ImGuiPass();
		void PreRender(DX12CommandList* commandList) override;
		void Render(DX12CommandList* commandList, RenderInterface* renderInterface, GameObjectTree* gameObjectTree, std::vector<LightObject*> lights) override;
		void SetupGlobalStyle();
		void PostRender(DX12CommandList* commandList) override;
	private:
		DX12Shader* pixelShader;
		DX12Shader* vertexShader;
	};
}