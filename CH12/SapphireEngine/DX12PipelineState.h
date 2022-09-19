#pragma once

#include <d3d12.h>
#include <d3dcompiler.h>

#include "DX12Shader.h"
#include "DX12Device.h"
#include "Logger.h"
#include "Utils.h"
#include "d3dx12.h"

#pragma comment(lib, "D3DCompiler.lib")

namespace Sapphire
{
	class DX12InputLayout;

	class DX12PipelineState
	{
		friend class DX12CommandList;
	public:
		DX12PipelineState(DX12Device* device, DX12Shader* vertexShader, DX12Shader* pixelShader, DX12InputLayout* inputLayout, bool flip = false);
		~DX12PipelineState();
	private:
		void CreateRootSignature(DX12Device* device);
		void CreatePipelineState(DX12Device* device, D3D12_SHADER_BYTECODE vs, D3D12_SHADER_BYTECODE ps, DX12InputLayout* inputLayout, bool flip = false);
		ID3D12RootSignature* rootSignature;
		ID3D12PipelineState* pipelineState;
	};
}