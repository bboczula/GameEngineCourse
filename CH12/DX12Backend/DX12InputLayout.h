#pragma once

#include <vector>
#include <d3d12.h>

namespace GpuApi
{
	enum class VertexStream
	{
		Position,
		Normal,
		Tangent,
		TexCoord
	};

	class DX12InputLayout
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