#include "DX12Viewport.h"

Sapphire::DX12Viewport::DX12Viewport(UINT width, UINT height)
	: viewport({ 0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, 1.0f })
{
}

Sapphire::DX12Viewport::~DX12Viewport()
{
}