#pragma once

#include <d3d12.h>
#include <d3dcompiler.h>

#include "DX12Shader.h"
#include "Logger.h"
#include "Utils.h"
#include "d3dx12.h"

#pragma comment(lib, "D3DCompiler.lib")

namespace Sapphire
{
	class DX12PipelineState
	{
		friend class DX12CommandList;
	public:
		DX12PipelineState(ID3D12Device* device, LPCWSTR shaderFileName);
		~DX12PipelineState();
	private:
		void CreateAndCompileShaders(LPCWSTR shaderFileName);
		void CreateRootSignature(ID3D12Device* device);
		void CreatePipelineState(ID3D12Device* device);
		ID3D12RootSignature* rootSignature;
		ID3D12PipelineState* pipelineState;
		DX12Shader* vs;
		DX12Shader* ps;
	};
}