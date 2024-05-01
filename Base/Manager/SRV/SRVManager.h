#pragma once
#include "Base/Common/DirectX/DirectXCommon.h"



struct DESCRIPTERHANDLE {
	D3D12_CPU_DESCRIPTOR_HANDLE CPU;
	D3D12_GPU_DESCRIPTOR_HANDLE GPU;
};
class SRVManager
{
public:
	SRVManager() {
		descriptorSizeSRV = DirectXCommon::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	}

	~SRVManager();

	DESCRIPTERHANDLE GetDescriptorHandle(uint32_t descriptorSize);
	static uint32_t descriptorSizeSRV;
private:
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(uint32_t descriptorSize);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(uint32_t descriptorSize);

	uint32_t SRVValue = 1;
};