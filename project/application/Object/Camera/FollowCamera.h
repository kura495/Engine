#pragma once
//ターゲットを追従するカメラ//
#include <random>
#include <numbers>

#include "Camera/Camera.h"
#include "Input/Input.h"
#include "WorldTransform/WorldTransform.h"
#include "Math/Matrix/MatrixCalc.h"
#include "Calc.h"
#include "Utility/random/random.h"

//カメラ補間用ワーク
struct WorkInterpolation {
	//追従対象の残像座標
	Vector3 interTarget_ = {};
	//追従対象のY軸
	float targetAngleY_ = 0.0f;
	//カメラ補間の媒介変数
	Vector3 interParameter_ = {1.0f,1.0f,1.0f};
};

class FollowCamera : public Camera
{
public:
	void Initialize()override;
	void Update()override;
	/// <summary>
	/// ターゲットを指定
	/// </summary>
	/// <param name="target">ターゲットのWorldTransform</param>
	void SetTarget(const WorldTransform* target);
	/// <summary>
	/// 
	/// </summary>
	void LockAt(const WorldTransform& target);

	void ReStert();

	void SetOffset(Vector3 offset) { offsetPos = offset; };

	static WorkInterpolation workInter;

#pragma region
	void Shake();
	bool isShake = false;
#pragma endregion シェイク
private:
	void ImGui();
	void ShakeFanction();
	float interParameter_ = 1.0f;

	Vector3 prePos_;
	Vector3 currentPos_;
	Vector3 lockVector;
	//追従対象
	const WorldTransform* target_ = nullptr;
	// ゲームパッド
	XINPUT_STATE joyState;

	Vector3 rotate_ = { 0.0f,0.0f,0.0f };

	//追従対象の座標・角度を再設定
	void Reset();

	//追従対象からのオフセットを計算する
	Vector3 OffsetCalc();

	Vector3 offsetPos = { 0.0f,0.0f,0.0f};

	float minShakeValue = -0.7f;
	float maxShakeValue =  0.7f;
};
