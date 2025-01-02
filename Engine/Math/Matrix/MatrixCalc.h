#pragma once
#include "Math_Structs.h"
#include "Math/Vector/VectorCalc.h"
#include <numbers>
#include <cmath>
using namespace Math;
	/// <summary>
	/// スケール値から拡縮行列を作成
	/// </summary>
	/// <param name="scale">スケール値</param>
	/// <returns>Matrix4x4</returns>
	Matrix4x4 MakeScaleMatrix(const Vector3& scale);
	/// <summary>
	/// ラジアンからX軸の回転行列を作成
	/// </summary>
	/// <param name="radian">ラジアン</param>
	/// <returns>Matrix4x4</returns>
	Matrix4x4 MakeRotateXMatrix(float radian);
	/// <summary>
	/// ラジアンからY軸の回転行列を作成
	/// </summary>
	/// <param name="radian">ラジアン</param>
	/// <returns>Matrix4x4</returns>
	Matrix4x4 MakeRotateYMatrix(float radian);
	/// <summary>
	/// ラジアンからZ軸の回転行列を作成
	/// </summary>
	/// <param name="radian">ラジアン</param>
	/// <returns>Matrix4x4</returns>
	Matrix4x4 MakeRotateZMatrix(float radian);
	/// <summary>
	/// 回転値から回転行列を作成
	/// </summary>
	/// <param name="rotation">回転値</param>
	/// <returns>Matrix4x4</returns>
	Matrix4x4 MakeRotateMatrix(Vector3 rotation);
	/// <summary>
	/// クォータニオンから回転行列を作成
	/// </summary>
	/// <param name="quaternion">クォータニオン</param>
	/// <returns>Matrix4x4</returns>
	Matrix4x4 MakeRotateMatrix(const Quaternion& quaternion);
	/// <summary>
	/// 移動値から移動行列を作成
	/// </summary>
	/// <param name="translate">移動値</param>
	/// <returns>Matrix4x4</returns>
	Matrix4x4 MakeTranslateMatrix(const Vector3& translate);
	/// <summary>
	/// スケール、ローテート、トランスレートから行列を作り合成したものを作成
	/// </summary>
	/// <param name="scale">スケール値</param>
	/// <param name="rotate">回転値</param>
	/// <param name="translate">移動値</param>
	/// <returns>Matrix4x4</returns>
	Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
	/// <summary>
	/// スケール、ローテート、トランスレートから行列を作り合成したものを作成
	/// </summary>
	/// <param name="scale">スケール値</param>
	/// <param name="rotate">クォータニオン</param>
	/// <param name="translate">移動値</param>
	/// <returns>Matrix4x4</returns>
	Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Quaternion& rotate, const Vector3& translate);
	/// <summary>
	/// クォータニオンから回転行列を求める
	/// </summary>
	/// <param name="quaternion">クォータニオン</param>
	/// <returns>Matrix4x4</returns>
	Matrix4x4 MakeRotateMatrix(const Quaternion& quaternion);
	/// <summary>
	/// 透視投影行列
	/// </summary>
	/// <param name="FovY">Y軸方向のFOV</param>
	/// <param name="aspectRatio">画面アスペクト比</param>
	/// <param name="nearClip">近くの描画制限</param>
	/// <param name="farClip">遠方の描画制限</param>
	/// <returns>Matrix4x4</returns>
	Matrix4x4 MakePerspectiveFovMatrix(float FovY, float aspectRatio, float nearClip, float farClip);
	/// <summary>
	/// ビューポート変換行列
	/// </summary>
	/// <param name="left">左</param>
	/// <param name="top">上</param>
	/// <param name="width">横幅</param>
	/// <param name="height">高さ</param>
	/// <param name="minDepth"></param>
	/// <param name="maxDepth"></param>
	/// <returns>Matrix4x4</returns>
	Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);
	/// <summary>
	/// cotangent関数
	/// タンジェントの逆比・逆数を求める
	/// </summary>
	/// <param name="top"></param>
	/// <returns>float</returns>
	float cot(float top);
	/// <summary>
	/// ウィンドウ座標で入力
	/// </summary>
	/// <param name="left">左</param>
	/// <param name="top">上</param>
	/// <param name="right">右</param>
	/// <param name="bottom">下</param>
	/// <param name="nearClip">近くの描画制限</param>
	/// <param name="farCcip">遠方の描画制限</param>
	/// <returns>Matrix4x4</returns>
	Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farCcip);
	/// <summary>
	/// ベクトルをクォータニオンで回転させた結果のベクトルを求める
	/// </summary>
	/// <param name="vector">回転させたいベクトル</param>
	/// <param name="quaternion">クォータニオン</param>
	/// <returns>Matrix4x4</returns>
	Vector3 TransformNormal(const Vector3& vector, const Quaternion& quaternion);
	/// <summary>
	/// ベクトルを回転行列で回転させた結果のベクトルを求める
	/// </summary>
	/// <param name="v">回転させたいベクトル</param>
	/// <param name="m">回転行列</param>
	/// <returns>Vector3</returns>
	Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);
	/// <summary>
	/// X軸の回転ベクトルを取得
	/// </summary>
	/// <param name="matrix">回転行列</param>
	/// <returns>Vector3</returns>
	Vector3 GetXAxis(Matrix4x4 matrix);
	/// <summary>
	/// Y軸の回転ベクトルを取得
	/// </summary>
	/// <param name="matrix">回転行列</param>
	/// <returns>Vector3</returns>
	Vector3 GetYAxis(Matrix4x4 matrix);
	/// <summary>
	/// Z軸の回転ベクトルを取得
	/// </summary>
	/// <param name="matrix">回転行列</param>
	/// <returns>Vector3</returns>
	Vector3 GetZAxis(Matrix4x4 matrix);
	/// <summary>
	/// ポイントからポイントへの向きから回転行列を作成する
	/// </summary>
	/// <param name="from">スタートポイント</param>
	/// <param name="to">向きたいポイント</param>
	/// <returns>Matrix4x4</returns>
	Matrix4x4 DirectionToDirection(const Vector3& from, const Vector3& to);
	/// <summary>	
	/// 軸を指定して回転量だけ回転した回転行列を生成
	/// </summary>	
	/// <param name="axis">軸</param>	
	/// <param name="angle">回転量</param>	
	/// <returns>Matrix4x4</returns>
	Matrix4x4 MakeRotateAxisAngle(const Vector3& axis, float angle);