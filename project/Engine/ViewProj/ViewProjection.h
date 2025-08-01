#pragma once
/*ビュープロジェクションクラス*/
#include "Common/DirectX/DirectXCommon.h"
#include "Math_Structs.h"
#include "Math/Matrix/MatrixCalc.h"
#include <d3d12.h>
#include <wrl.h>
using namespace Math;
// 定数バッファ用データ構造体
struct ConstBufferDataViewProjection {
	Matrix4x4 view;       // ワールド → ビュー変換行列
	Matrix4x4 projection; // ビュー → プロジェクション変換行列
	Vector3 cameraPos;    // カメラ座標（ワールド座標）
};
struct ConstBufferCameraPos {
	Vector3 worldPosition;
};
struct CameraParameter {
	// X,Y,Z軸回りのローカル回転角
	Quaternion rotation_ = { 0, 0, 0, 1 };
	// ローカル座標
	Vector3 translation_ = { 0.0f, 0.0f, 0.0f };
	// 垂直方向視野角
	float FOV = 45.0f;
};
/// <summary>
/// ビュープロジェクション変換データ
/// </summary>
struct ViewProjection {
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_VS;
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_PS;
	// マッピング済みアドレス
	ConstBufferDataViewProjection* constMap_VS = nullptr;
	ConstBufferCameraPos* constMap_PS = nullptr;

#pragma region ビュー行列の設定
	CameraParameter parameter;
#pragma endregion

#pragma region 透視投影行列の設定

	// ビューポートのアスペクト比
	float aspectRatio = (float)16 / 9;
	// 深度限界（手前側）
	float nearZ = 0.1f;
	// 深度限界（奥側）
	float farZ = 1000.0f;
#pragma endregion

	// ビュー行列
	Matrix4x4 matView={};
	// 透視投影行列
	Matrix4x4 matProjection={};
	// カメラ行列
	Matrix4x4 CameraMatrix={};

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 定数バッファ生成
	/// </summary>
	void CreateConstBuffer();
	/// <summary>
	/// マッピングする
	/// </summary>
	void Map();
	/// <summary>
	/// 行列を更新する
	/// </summary>
	void Update();
	/// <summary>
	/// 行列を転送する
	/// </summary>
	void TransferMatrix();
	/// <summary>
	/// ビュー行列を更新する
	/// </summary>
	void UpdateViewMatrix();
	/// <summary>
	/// 射影行列を更新する
	/// </summary>
	void UpdateProjectionMatrix();
};