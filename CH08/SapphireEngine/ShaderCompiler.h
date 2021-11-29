#pragma once

#include <d3d12.h>
#include <dxcapi.h>

#include "Utils.h"
#include "Logger.h"

#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "dxcompiler.lib")

namespace Sapphire
{
	class ShaderCompiler
	{
	public:
		ShaderCompiler();
		~ShaderCompiler();
		IDxcOperationResult* Compile(LPCWSTR fileName, LPCWSTR entryPoint, LPCWSTR shaderModel);
	private:
		void HandleCompilationError(IDxcOperationResult* result);
		IDxcLibrary* library;
		IDxcCompiler* compiler;
	};
}