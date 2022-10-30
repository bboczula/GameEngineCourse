#pragma once

#include <Windows.h>

#include "DxgiManager.h"
#include "../DX12Backend/DX12Device.h"
#include "../DX12Backend/DX12CommandQueue.h"
#include "../DX12Backend/DX12CommandList.h"
#include "../DX12Backend/DX12Resource.h"
#include "../DX12Backend/DX12DescriptorHeap.h"
#include "Utils.h"

#include "../DX12Backend/DX12Device.h"
#include "../DX12Backend/DX12CommandQueue.h"
#include "../DX12Backend/DX12CommandList.h"
#include "../DX12Backend/DX12Resource.h"
#include "../DX12Backend/DX12DescriptorHeap.h"

//#pragma comment(lib, "DX12Backend.lib")

namespace Sapphire
{
	struct TempRendererSettings
	{
		bool isVsyncEnabled = true;
	};

	class DeviceContext
	{
	public:
		DeviceContext(HWND hwnd, unsigned int width, unsigned int height);
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