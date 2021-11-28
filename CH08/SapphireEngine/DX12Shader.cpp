#include "DX12Shader.h"

Sapphire::DX12Shader::DX12Shader(LPCWSTR fileName, SHADER_TYPE type) : bytecode({ nullptr, 0 }), type(type)
{
	switch (type)
	{
	case SHADER_TYPE::VERTEX_SHADER:
		Compile(fileName, L"VSMain", L"vs_6_0");
		break;
	case SHADER_TYPE::PIXEL_SHADER:
		Compile(fileName, L"PSMain", L"ps_6_0");
		break;
	default:
		break;
	}
}

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

	// Not initializing this
	IDxcOperationResult* result = nullptr;
	HRESULT compilationResult = 0;
	if (sourceBlob != nullptr)
	{
		// Here warning was generated without this check
		compilationResult = compiler->Compile(sourceBlob, fileName, entryPoint, shaderModel, NULL, 0, NULL, 0, NULL, &result);
	}

	if (FAILED(compilationResult))
	{
		HandleCompilationError(result);
	}
	ProcessAndStoreResult(result);

	Logger::GetInstance().Log("Shader %hs compiled successfully.\n", fileName);
}

void Sapphire::DX12Shader::ProcessAndStoreResult(IDxcOperationResult* result)
{
	IDxcBlob* code;
	result->GetResult(&code);
	bytecode.pShaderBytecode = code->GetBufferPointer();
	bytecode.BytecodeLength = code->GetBufferSize();
	SafeRelease(&code);
}

void Sapphire::DX12Shader::HandleCompilationError(IDxcOperationResult* result)
{
	IDxcBlobEncoding* errorsBlob;
	ExitIfFailed(result->GetErrorBuffer(&errorsBlob));
	if (errorsBlob)
	{
		// Here log compilation errors
		Logger::GetInstance().Log("HLSL Compilation: %hs", (const char*)errorsBlob->GetBufferPointer());
	}
	exit(2);
}

D3D12_SHADER_BYTECODE Sapphire::DX12Shader::GetBytecode()
{
	return bytecode;
}
