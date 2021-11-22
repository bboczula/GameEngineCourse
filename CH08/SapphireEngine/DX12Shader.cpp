#include "DX12Shader.h"

Sapphire::DX12Shader::DX12Shader()
{
}

Sapphire::DX12Shader::~DX12Shader()
{
}

void Sapphire::DX12Shader::Compile(LPCWSTR fileName, LPCSTR entryPoint, LPCSTR shaderModel)
{
#if defined(_DEBUG)
	// Enable better shader debugging with the graphics debugging tools.
	UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	UINT compileFlags = 0;
#endif

	ExitIfFailed(D3DCompileFromFile(fileName, nullptr, nullptr, entryPoint, shaderModel, compileFlags, 0, &shader, nullptr));

	bytecode.pShaderBytecode = shader->GetBufferPointer();
	bytecode.BytecodeLength = shader->GetBufferSize();
}

D3D12_SHADER_BYTECODE Sapphire::DX12Shader::GetBytecode()
{
	return bytecode;
}
