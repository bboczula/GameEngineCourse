#pragma once

#include "Utils.h"

#include <d3d12.h>
#include <d3dcompiler.h>

#pragma comment(lib, "D3DCompiler.lib")

namespace Sapphire
{
	// Remember to write about changing HLSL file type
	class DX12Shader
	{
	public:
		DX12Shader();
		~DX12Shader();
		void Compile(LPCWSTR fileName, LPCSTR entryPoint, LPCSTR shaderModel);
		D3D12_SHADER_BYTECODE GetBytecode();
	private:
		ID3DBlob* shader;
		D3D12_SHADER_BYTECODE bytecode;
	};
}