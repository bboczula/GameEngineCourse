#pragma once

#include "Utils.h"
#include "Logger.h"
#include "InputFile.h"

#include <d3d12.h>

namespace Sapphire
{
	/// <summary>
	/// Keeps the compiled shader data for Pipeline State setup.
	/// </summary>
	class DX12Shader
	{
	public:
		DX12Shader(LPCSTR fileName);
		~DX12Shader();
		D3D12_SHADER_BYTECODE GetBytecode();
	private:
		D3D12_SHADER_BYTECODE bytecode;
	};
}