#pragma once

#include <Windows.h>
#include <vector>

namespace Sapphire
{
	class RenderInterface;
	class ShadowMapPass;
	class ForwardRenderingPass;
	class GrayscalePass;
	class DeferredRenderingPass;
	class LightResolvePass;
	class GameObject;
	class LightObject;
	class Camera;
	class RenderPass;
	class ImGuiPass;
	class GameObjectTree;

	class Renderer
	{
	public:
		Renderer(HWND hwnd, UINT width, UINT height);
		~Renderer();
		void Render(GameObjectTree* gameObjectTree, std::vector<LightObject*> lights);
		void SetCamera(Camera* camera);
		void CreateResources(GameObjectTree* gameObjectTree);
		void Execute();
	private:
		RenderInterface* renderInterface;
		std::vector<RenderPass*> renderPasses;
		ShadowMapPass* shadowMapPass;
		ForwardRenderingPass* forwardRenderingPass;
		GrayscalePass* grayscalePass;
		DeferredRenderingPass* defferedRenderingPass;
		LightResolvePass* lightResolvePass;
		ImGuiPass* imGuiPass;
	};
}