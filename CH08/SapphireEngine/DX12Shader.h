#pragma once

#include "Utils.h"
#include "Logger.h"
#include "ShaderCompiler.h"

namespace Sapphire
{
	enum class SHADER_TYPE
	{
		VERTEX_SHADER,
		PIXEL_SHADER,
		UNDEFINED
	};

	// Remember to write about changing HLSL file type
	class DX12Shader
	{
	public:
		DX12Shader();
		~DX12Shader();
		void Compile(LPCWSTR fileName, SHADER_TYPE type, ShaderCompiler* compiler);
		D3D12_SHADER_BYTECODE GetBytecode();
	private:
		void Compile(LPCWSTR fileName, LPCWSTR entryPoint, LPCWSTR shaderModel, ShaderCompiler* compiler);
		void ProcessAndStoreResult(IDxcOperationResult* result);
		SHADER_TYPE type;
		D3D12_SHADER_BYTECODE bytecode;
	};
}