#pragma once
//シェーダーリソースビューの管理クラス//
#include "Common/DirectX/DirectXCommon.h"
#include "externals/DirectXTex/DirectXTex.h"

struct DESCRIPTERHANDLE {
	CD3DX12_CPU_DESCRIPTOR_HANDLE CPU;
	CD3DX12_GPU_DESCRIPTOR_HANDLE GPU;
};
class SRVManager
{
public:

	static SRVManager* GetInstance();
	/// <summary>
	/// 一番後ろのSRVハンドルを取得
	/// </summary>
	/// <returns>DESCRIPTERHANDLE</returns>
	DESCRIPTERHANDLE GetDescriptorHandle();
	uint32_t descriptorSizeSRV;
private:
	SRVManager() { 
		descriptorSizeSRV = DirectXCommon::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	}
	~SRVManager() = default;
	SRVManager(const SRVManager& obj) = delete;
	SRVManager& operator=(const SRVManager& obj) = delete;
	/// <summary>
	/// 現在のハンドルの数を計算
	/// </summary>
	/// <returns>D3D12_CPU_DESCRIPTOR_HANDLE</returns>
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle();
	/// <summary>
	/// 現在のハンドルの数を計算
	/// </summary>
	/// <returns>D3D12_GPU_DESCRIPTOR_HANDLE</returns>
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle();

	uint32_t SRVValue = 1;
};