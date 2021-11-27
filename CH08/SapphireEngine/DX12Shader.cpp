#include "DX12Shader.h"

Sapphire::DX12Shader::DX12Shader(LPCWSTR fileName, LPCWSTR entryPoint, LPCWSTR shaderModel)
{
	Compile(fileName, entryPoint, shaderModel);
}

Sapphire::DX12Shader::~DX12Shader()
{
}

void Sapphire::DX12Shader::Compile(LPCWSTR fileName, LPCWSTR entryPoint, LPCWSTR shaderModel)
{
	IDxcLibrary* library;
	ExitIfFailed(DxcCreateInstance(CLSID_DxcLibrary, IID_PPV_ARGS(&library)));

	IDxcCompiler* compiler;
	ExitIfFailed(DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&compiler)));

	uint32_t codePage = CP_UTF8;
	IDxcBlobEncoding* sourceBlob;
	ExitIfFailed(library->CreateBlobFromFile(fileName, &codePage, &sourceBlob));

	IDxcOperationResult* result;
	HRESULT compilationResult = compiler->Compile(sourceBlob, fileName, entryPoint, shaderModel, NULL, 0, NULL, 0, NULL, &result);

	if (SUCCEEDED(compilationResult))
	{
		result->GetStatus(&compilationResult);
	}

	if (FAILED(compilationResult))
	{
		IDxcBlobEncoding* errorsBlob;
		ExitIfFailed(result->GetErrorBuffer(&errorsBlob));
		if (errorsBlob)
		{
			// Here log compilation errors
			Logger::GetInstance().Log("HLSL Compilation Error: %hs", (const char*)errorsBlob->GetBufferPointer());
		}
		// Handle compilation errors
	}

	IDxcBlob* code;
	result->GetResult(&code);
	bytecode.pShaderBytecode = code->GetBufferPointer();
	bytecode.BytecodeLength = code->GetBufferSize();
}

D3D12_SHADER_BYTECODE Sapphire::DX12Shader::GetBytecode()
{
	return bytecode;
}
