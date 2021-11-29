#include "DX12Shader.h"

Sapphire::DX12Shader::DX12Shader() : bytecode({ nullptr, 0 }), type(SHADER_TYPE::UNDEFINED)
{
	Logger::GetInstance().Log("%s\n", "Sapphire::DX12Shader::DX12Shader()");
}

Sapphire::DX12Shader::~DX12Shader()
{
	Logger::GetInstance().Log("%s\n", "Sapphire::DX12Shader::~DX12Shader()");
}

void Sapphire::DX12Shader::Compile(LPCWSTR fileName, SHADER_TYPE type, ShaderCompiler* compiler)
{
	Logger::GetInstance().Log("%s\n", "Sapphire::DX12Shader::Compile()");

	// Store the shader type
	this->type = type;

	switch (type)
	{
	case SHADER_TYPE::VERTEX_SHADER:
		Compile(fileName, L"VSMain", L"vs_6_0", compiler);
		break;
	case SHADER_TYPE::PIXEL_SHADER:
		Compile(fileName, L"PSMain", L"ps_6_0", compiler);
		break;
	default:
		break;
	}
}

void Sapphire::DX12Shader::Compile(LPCWSTR fileName, LPCWSTR entryPoint, LPCWSTR shaderModel, ShaderCompiler* compiler)
{
	IDxcOperationResult* result = compiler->Compile(fileName, entryPoint, shaderModel);
	ProcessAndStoreResult(result);

	Logger::GetInstance().Log("Shader %ls compiled successfully.\n", fileName);
}

void Sapphire::DX12Shader::ProcessAndStoreResult(IDxcOperationResult* result)
{
	Logger::GetInstance().Log("%s\n", "Sapphire::DX12Shader::ProcessAndStoreResult()");

	IDxcBlob* code;
	result->GetResult(&code);
	bytecode.pShaderBytecode = code->GetBufferPointer();
	bytecode.BytecodeLength = code->GetBufferSize();
	SafeRelease(&code);
}

D3D12_SHADER_BYTECODE Sapphire::DX12Shader::GetBytecode()
{
	Logger::GetInstance().Log("%s\n", "Sapphire::DX12Shader::GetBytecode()");

	return bytecode;
}