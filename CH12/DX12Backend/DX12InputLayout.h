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

	extern class GPUAPI_DX12_API DX12InputLayout
	{
	public:
		DX12InputLayout();
		~DX12InputLayout();
		void AppendElement(VertexStream vertexStream);
		D3D12_INPUT_LAYOUT_DESC GetInputLayoutDesc();
	private:
		std::vector<D3D12_INPUT_ELEMENT_DESC> inputElementsList;
	};
}