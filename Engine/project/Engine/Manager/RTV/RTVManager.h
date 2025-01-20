#pragma once
//レンダーターゲットビューの管理クラス//
#include "Common/DirectX/DirectXCommon.h"
#include "externals/DirectXTex/DirectXTex.h"

class RTVManager
{
public:

	static RTVManager* GetInstance();
	/// <summary>
	/// 一番後ろのRTVハンドルを取得
	/// </summary>
	/// <returns>D3D12_CPU_DESCRIPTOR_HANDLE</returns>
	D3D12_CPU_DESCRIPTOR_HANDLE GetDescriptorHandle();

	uint32_t descriptorSizeRTV;
private:
	RTVManager() {
		descriptorSizeRTV = DirectXCommon::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}
	~RTVManager() = default;
	RTVManager(const RTVManager& obj) = delete;
	RTVManager& operator=(const RTVManager& obj) = delete;
	/// <summary>
	/// 現在のハンドルの数を計算
	/// </summary>
	/// <returns></returns>
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle();

	uint32_t RTVValue = 0;
};