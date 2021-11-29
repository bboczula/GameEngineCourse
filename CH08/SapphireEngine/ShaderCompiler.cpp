#include "ShaderCompiler.h"

Sapphire::ShaderCompiler::ShaderCompiler()
{
	Logger::GetInstance().Log("%s\n", "Sapphire::ShaderCompiler::ShaderCompiler()");

	ExitIfFailed(DxcCreateInstance(CLSID_DxcLibrary, IID_PPV_ARGS(&library)));
	ExitIfFailed(DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&compiler)));
}

Sapphire::ShaderCompiler::~ShaderCompiler()
{
	Logger::GetInstance().Log("%s\n", "Sapphire::ShaderCompiler::~ShaderCompiler()");

	SafeRelease(&library);
	SafeRelease(&compiler);
}

IDxcOperationResult* Sapphire::ShaderCompiler::Compile(LPCWSTR fileName, LPCWSTR entryPoint, LPCWSTR shaderModel)
{
	Logger::GetInstance().Log("%s(%ls, %ls, %ls)\n", "Sapphire::ShaderCompiler::Compile", fileName, entryPoint, shaderModel);

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

	return result;
}

void Sapphire::ShaderCompiler::HandleCompilationError(IDxcOperationResult* result)
{
	Logger::GetInstance().Log("%s\n", "Sapphire::ShaderCompiler::HandleCompilationError()");

	IDxcBlobEncoding* errorsBlob;
	ExitIfFailed(result->GetErrorBuffer(&errorsBlob));
	if (errorsBlob)
	{
		Logger::GetInstance().Log("Shader Compilation: %hs", (const char*)errorsBlob->GetBufferPointer());
	}
	exit(2);
}
