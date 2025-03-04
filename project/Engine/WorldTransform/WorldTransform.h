﻿#pragma once
/*トランスフォームクラス*/
#include "Math_Structs.h"
#include "Math/Matrix/MatrixCalc.h"
#include "Common/DirectX/DirectXCommon.h"
#include "Transform.h"
class Model;
struct ConstBufferDataWorldTransform {
	Matrix4x4 matWorld; // ローカル → ワールド変換行列
	Matrix4x4 WorldInverseTranspose;
};
struct WorldTransform {
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_ = nullptr;
	// マッピング済みアドレス
	ConstBufferDataWorldTransform* constMap = nullptr;

	TransformQua transform = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f,1.0f},
		{0.0f,0.0f,0.0f}
	};

	// ローカル → ワールド変換行列
	Matrix4x4 matWorld_;
	// 親となるワールド変換へのポインタ
	const WorldTransform* parent_ = nullptr;

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	/// <summary>
	/// 定数バッファ生成
	/// </summary>
	void CreateConstBuffer();
	/// <summary>
	/// マッピングする
	/// </summary>
	void Map();
	/// <summary>
	/// 行列を転送する
	/// </summary>
	void TransferMatrix();

	void Update();
	void UpdateMatrix(Matrix4x4 matrix);
	/// <summary>
	/// matWorldから移動量を抜き取る関数
	/// </summary>
	Vector3 GetTranslateFromMatWorld()const;

	void SetParent(const WorldTransform* parent);

	void SetTransform(Model* model);
};