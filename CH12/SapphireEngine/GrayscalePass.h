#pragma once

#include "RenderPass.h"

namespace Sapphire
{
	class RenderInterface;
	class DX12Shader;
	class GameObject;
	class GameObjectTree;

	class GrayscalePass : public RenderPass
	{
	public:
		GrayscalePass(RenderInterface* renderInterface, unsigned int width, unsigned int height);
		~GrayscalePass();
		void PreRender(DX12CommandList* commandList) override;
		void Render(DX12CommandList* commandList, RenderInterface* renderInterface, GameObjectTree* gameObjectTree, std::vector<LightObject*> lights) override;
		void PostRender(DX12CommandList* commandList) override;
	private:
		DX12Shader* pixelShader;
		DX12Shader* vertexShader;
	};
}