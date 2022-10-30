#include "pch.h"

#include "DX12Shader.h"

Sapphire::DX12Shader::DX12Shader(LPCSTR fileName) : bytecode({ nullptr, 0 })
{
	InputFile compiledShader;
	compiledShader.Open(fileName);
	compiledShader.Read();
	compiledShader.Close();

	bytecode.BytecodeLength = compiledShader.GetSize();
	bytecode.pShaderBytecode = compiledShader.GetBuffer();
}

Sapphire::DX12Shader::~DX12Shader()
{
	// This class is responsible for that blob
	delete bytecode.pShaderBytecode;
}

D3D12_SHADER_BYTECODE Sapphire::DX12Shader::GetBytecode()
{
	return bytecode;
}