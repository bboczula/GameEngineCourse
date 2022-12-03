#include "pch.h"

#include "DX12MultiRenderTarget.h"
#include "DX12RenderTarget.h"

Sapphire::DX12MultiRenderTarget::DX12MultiRenderTarget()
{
}

void Sapphire::DX12MultiRenderTarget::Add(DX12RenderTarget* renderTarget)
{
	renderTargets.push_back(renderTarget);
	descriptorHandles.push_back(renderTarget->descriptorHandle);
}

Sapphire::DX12RenderTarget* Sapphire::DX12MultiRenderTarget::Get(unsigned int index)
{
	return renderTargets[index];
}

unsigned int Sapphire::DX12MultiRenderTarget::Size()
{
	auto a = renderTargets.data();
	return renderTargets.size();
}

Sapphire::DX12RenderTarget** Sapphire::DX12MultiRenderTarget::Data()
{
	return renderTargets.data();
}

D3D12_CPU_DESCRIPTOR_HANDLE* Sapphire::DX12MultiRenderTarget::GetDescriptorHandlesArray()
{
	return descriptorHandles.data();
}
