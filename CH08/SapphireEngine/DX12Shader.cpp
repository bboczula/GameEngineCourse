#include "DX12Shader.h"

Sapphire::DX12Shader::DX12Shader(LPCSTR fileName) : bytecode({ nullptr, 0 })
{
	Logger::GetInstance().Log("%s\n", "Sapphire::DX12Shader::DX12Shader()");

	InputFile compiledShader;
	compiledShader.Open(fileName);
	compiledShader.Read();
	compiledShader.Close();

	bytecode.BytecodeLength = compiledShader.GetSize();
	bytecode.pShaderBytecode = compiledShader.GetBuffer();
}

Sapphire::DX12Shader::~DX12Shader()
{
	Logger::GetInstance().Log("%s\n", "Sapphire::DX12Shader::~DX12Shader()");

	// This class is responsible for that blob
	delete bytecode.pShaderBytecode;
}

D3D12_SHADER_BYTECODE Sapphire::DX12Shader::GetBytecode()
{
	Logger::GetInstance().Log("%s\n", "Sapphire::DX12Shader::GetBytecode()");

	return bytecode;
}