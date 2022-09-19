#pragma once

#include <Windows.h>

#include "DxgiManager.h"
#include "DX12Device.h"
#include "DX12CommandQueue.h"
#include "DX12CommandList.h"
#include "DX12Resource.h"
#include "DX12DescriptorHeap.h"
#include "Utils.h"

namespace Sapphire
{
	struct TempRendererSettings
	{
		bool isVsyncEnabled = true;
	};

	class DeviceContext
	{
	public:
		DeviceContext(HWND hwnd);
		~DeviceContext();
		DX12Resource* GetBackBuffer(UINT index);
		UINT GetCurrentFrameIndex();
		void Present();
		DX12Device* GetDevice();
		void Execute(DX12CommandList* commandList);
		DX12DescriptorHeap* CreateRtvDescriptorHeap();
		DX12DescriptorHeap* CreateDsvDescriptorHeap();
		DX12DescriptorHeap* CreateSrvDescriptorHeap();
		DX12CommandList* CreateCommandList();
	private:
		TempRendererSettings settings;
		DxgiManager* dxgiManager;
		DX12Device* device;
		DX12CommandQueue* commandQueue;
	};
}