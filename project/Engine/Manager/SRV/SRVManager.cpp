#include "SRVManager.h"

SRVManager* SRVManager::GetInstance()
{
	static SRVManager instance;
	return &instance;
}

DESCRIPTERHANDLE SRVManager::GetDescriptorHandle()
{
	DESCRIPTERHANDLE result{};
	result.CPU = GetCPUDescriptorHandle();
	result.GPU = GetGPUDescriptorHandle();
	SRVValue += 1;
	assert(SUCCEEDED(SRVValue <= 120));
	return result;
}

D3D12_CPU_DESCRIPTOR_HANDLE SRVManager::GetCPUDescriptorHandle()
{
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = DirectXCommon::GetInstance()->GetsrvDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSizeSRV * SRVValue);
	return handleCPU;
}
D3D12_GPU_DESCRIPTOR_HANDLE SRVManager::GetGPUDescriptorHandle()
{
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = DirectXCommon::GetInstance()->GetsrvDescriptorHeap()->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSizeSRV * SRVValue);
	return handleGPU;
}
