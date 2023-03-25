#pragma once

#include <Windows.h>
#include <vector>

namespace Sapphire
{
	class RenderInterface;
	class Light;
	class ShadowMapPass;
	class ForwardRenderingPass;
	class GrayscalePass;
	class DeferredRenderingPass;
	class LightResolvePass;
	class GameObject;
	class Camera;

	class Renderer
	{
	public:
		Renderer(HWND hwnd, UINT width, UINT height);
		~Renderer();
		void Render(std::vector<GameObject*> objects);
		void SetCamera(Camera* camera);
		void CreateResources(std::vector<GameObject*> objects);
		void Execute();
	private:
		RenderInterface* renderInterface;
		Light* directionalLight;
		ShadowMapPass* shadowMapPass;
		ForwardRenderingPass* forwardRenderingPass;
		GrayscalePass* grayscalePass;
		DeferredRenderingPass* defferedRenderingPass;
		LightResolvePass* lightResolvePass;
	};
}