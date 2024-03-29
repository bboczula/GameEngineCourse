#pragma once

#include "RenderPass.h"

namespace Sapphire
{
	class GameObjectTree;

	class LightResolvePass : public Sapphire::RenderPass
	{
	public:
		LightResolvePass(RenderInterface* renderInterface, unsigned int width, unsigned int height);
		~LightResolvePass();
		void PreRender(DX12CommandList* commandList) override;
		void Render(DX12CommandList* commandList, RenderInterface* renderInterface, GameObjectTree* gameObjectTree, std::vector<LightObject*> lights)  override;
		void PostRender(DX12CommandList* commandList)  override;
	private:
		DX12Shader* pixelShader;
		DX12Shader* vertexShader;
	};
}