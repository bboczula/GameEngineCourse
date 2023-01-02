#pragma once

#ifdef DX12BACKEND_EXPORTS
#define GPUAPI_DX12_API __declspec(dllexport)
#else
#define GPUAPI_DX12_API __declspec(dllimport)
#endif

#include <vector>
#include <d3d12.h>

namespace Sapphire
{
	enum class GPUAPI_DX12_API VertexStream
	{
		Position,
		Normal,
		Tangent,
		TexCoord
	};

	class GPUAPI_DX12_API DX12InputLayout
	{
	public:
		DX12InputLayout();
		~DX12InputLayout();
		template <typename T, typename... Types>
		void AppendElementT(T firstArg, Types... restArgs)
		{
			AppendElement(firstArg);
			AppendElementT(restArgs...);
		}
		template<typename T>
		void AppendElementT(T firstArg)
		{
			AppendElement(firstArg);
		}
		D3D12_INPUT_LAYOUT_DESC GetInputLayoutDesc();
	private:
		void AppendElement(VertexStream vertexStream);
		std::vector<D3D12_INPUT_ELEMENT_DESC> inputElementsList;
	};
}