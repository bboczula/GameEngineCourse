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
		/// <summary>
		/// Creates new shader instance using compiled shader file.
		/// </summary>
		/// <param name="source">Name of the file containing compiled shader code.</param>
		DX12Shader(LPCSTR fileName);

		~DX12Shader();

		/// <summary>
		/// Returns a copy of the compiled shader code and its size.
		/// </summary>
		D3D12_SHADER_BYTECODE GetBytecode();
	private:
		D3D12_SHADER_BYTECODE bytecode;
	};
}