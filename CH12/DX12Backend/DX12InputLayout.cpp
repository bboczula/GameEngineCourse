#include "pch.h"

#include "DX12InputLayout.h"

GpuApi::DX12InputLayout::DX12InputLayout()
{
}

GpuApi::DX12InputLayout::~DX12InputLayout()
{
}

void GpuApi::DX12InputLayout::AppendElement(VertexStream vertexStream)
{
	D3D12_INPUT_ELEMENT_DESC element;
	ZeroMemory(&element, sizeof(D3D12_INPUT_ELEMENT_DESC));

	element.InputSlot = inputElementsList.size();
	element.SemanticIndex = 0;
	element.InstanceDataStepRate = 0;
	element.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
	element.AlignedByteOffset = 0;

	switch (vertexStream)
	{
	case VertexStream::Position:
		element.SemanticName = "POSITION";
		element.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		break;
	case VertexStream::Normal:
		element.SemanticName = "NORMAL";
		element.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		break;
	case VertexStream::Tangent:
		element.SemanticName = "TANGENT";
		element.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		break;
	case VertexStream::TexCoord:
		element.SemanticName = "TEXCOORD";
		element.Format = DXGI_FORMAT_R32G32_FLOAT;
		break;
	default:
		element.SemanticName = "UNDEFINED";
		element.Format = DXGI_FORMAT_UNKNOWN;
		break;
	}

	inputElementsList.push_back(element);
}

D3D12_INPUT_LAYOUT_DESC GpuApi::DX12InputLayout::GetInputLayoutDesc()
{
	D3D12_INPUT_LAYOUT_DESC inputLayout;
	inputLayout.pInputElementDescs = inputElementsList.data();
	inputLayout.NumElements = inputElementsList.size();
	return inputLayout;
}
