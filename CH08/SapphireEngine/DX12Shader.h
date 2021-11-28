#pragma once

#include "Utils.h"
#include "Logger.h"

#include <d3d12.h>
#include <d3dcompiler.h>

// New compiler
#include <dxcapi.h>

#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "dxcompiler.lib")

namespace Sapphire
{
	enum class SHADER_TYPE
	{
		VERTEX_SHADER,
		PIXEL_SHADER
	};

	// Remember to write about changing HLSL file type
	class DX12Shader
	{
		//friend class DX12PipelineState;
	public:
		DX12Shader(LPCWSTR fileName, SHADER_TYPE type);
		DX12Shader(LPCWSTR fileName, LPCWSTR entryPoint, LPCWSTR shaderModel);
		~DX12Shader();
		D3D12_SHADER_BYTECODE GetBytecode();
	private:
		void Compile(LPCWSTR fileName, LPCWSTR entryPoint, LPCWSTR shaderModel);
		void ProcessAndStoreResult(IDxcOperationResult* result);
		void HandleCompilationError(IDxcOperationResult* result);
		SHADER_TYPE type;
		D3D12_SHADER_BYTECODE bytecode;
	};
}