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
class WorkLockAt {
public:
	/// <summary>
	/// ターゲットの方向に線形補間をしながら指定した軸を回転
	/// </summary>
	/// <param name="cameraRatVector">回転させるオイラー角</param>
	void Update(float& cameraRatVector) {
		if (flag) {
			easeT = (std::min)(easeT + AddEaseT, 1.0f);
			cameraRatVector = LerpShortAngle(cameraRatVector, cameraRatVector + lockAtRat, easeT);
			if (easeT == 1.0f) {
				flag = false;
			}
		}
	}
	//有効化フラグ
	bool flag = false;
	//回転
	float lockAtRat = 0.0f;
	//現在のTの値
	float easeT = 0.0f;
	//raseTに毎フレーム加算する値
	float AddEaseT = 0.02f;
};
class WorkFOV {
public:
	/// <summary>
	/// FOVを線形補間しながら変更
	/// </summary>
	/// <param name="FOV"></param>
	void Update(float& FOV) {
		if (flag) {
			easeT = (std::min)(easeT + AddEaseT, 1.0f);
			FOV = Lerp(FOV, newFOVvalue, easeT);
			if (easeT == 1.0f) {
				flag = false;
			}
		}
	}
	//有効化フラグ
	bool flag = false;
	//変更する値
	float newFOVvalue = 0.0f;
	//現在のTの値
	float easeT = 0.0f;
	//raseTに毎フレーム加算する値
	float AddEaseT = 0.02f;
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

	void SetFOV(float FOVvalue);

	static WorkInterpolation workInter;
	void ImGui();
#pragma region
	void Shake();
	bool isShake = false;
#pragma endregion シェイク
private:

	void ShakeFanction();
	float interParameter_ = 1.0f;

	Vector3 prePos_;
	Vector3 currentPos_;

	WorkLockAt workLockAt;
	WorkFOV workFOV;
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
