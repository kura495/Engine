#include "SRVManager.h"

uint32_t descriptorSizeSRV;

DESCRIPTERHANDLE SRVManager::GetDescriptorHandle(uint32_t descriptorSize)
{
	DESCRIPTERHANDLE result;
	result.CPU = GetCPUDescriptorHandle(descriptorSize);
	result.GPU = GetGPUDescriptorHandle(descriptorSize);
	SRVValue += 1;
	return result;
}

D3D12_CPU_DESCRIPTOR_HANDLE SRVManager::GetCPUDescriptorHandle(uint32_t descriptorSize)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = DirectXCommon::GetInstance()->GetsrvDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize * SRVValue);
	return handleCPU;
}
D3D12_GPU_DESCRIPTOR_HANDLE SRVManager::GetGPUDescriptorHandle(uint32_t descriptorSize)
{
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = DirectXCommon::GetInstance()->GetsrvDescriptorHeap()->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize * SRVValue);
	return handleGPU;
}
