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

	class Renderer
	{
	public:
		Renderer(HWND hwnd, UINT width, UINT height);
		~Renderer();
		void Render(std::vector<GameObject*> objects, std::vector<LightObject*> lights);
		void SetCamera(Camera* camera);
		void CreateResources(std::vector<GameObject*> objects);
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