#pragma once

#ifdef DX12BACKEND_EXPORTS
#define GPUAPI_DX12_API __declspec(dllexport)
#else
#define GPUAPI_DX12_API __declspec(dllimport)
#endif

#include "Utils.h"
#include "InputFile.h"

#include <d3d12.h>

namespace Sapphire
{
	extern class GPUAPI_DX12_API DX12Shader
	{
	public:
		DX12Shader(LPCSTR fileName);
		~DX12Shader();
		D3D12_SHADER_BYTECODE GetBytecode();
	private:
		D3D12_SHADER_BYTECODE bytecode;
	};
}