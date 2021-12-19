#pragma once

#include "Utils.h"
#include "Logger.h"

#include <d3d12.h>
#include <dxgi1_5.h>

namespace Sapphire
{
	/// <summary>
	/// Encapsulates behavior around the DirectX 12 device creation.
	/// </summary>
	class DX12Device
	{
	public:
		/// <summary>
		/// Creates instance of the DirectX 12 Device.
		/// </summary>
		/// <param name="adapter">DXGI adapter to bound to the new Device</param>
		DX12Device(IDXGIAdapter1* adapter);

		~DX12Device();

		/// <summary>
		/// Returns raw pointer to the Device.
		/// </summary>
		ID3D12Device* GetDevice();
	private:
		/// <summary>
		/// Enable debug layers, both on the API-level and GPU-level.
		/// </summary>
		void EnableDebugLayer();
		ID3D12Device* device;
	};
}