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
	//線形補間の1フレームごとの加山地
	float addeaseT = 0.0002f;

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
class WorkShake {
public:
	void Update(Vector3& translate) {
		if (flag) {
			easeT = (std::min)(easeT + kDeltaTime, timer);
			
			Vector3 ramdomTranslate = { random::Generate<float>(minShakeValue,maxShakeValue),random::Generate<float>(minShakeValue,maxShakeValue),random::Generate<float>(minShakeValue, maxShakeValue) };

			translate += ramdomTranslate;
			
			if (easeT == timer) {
				flag = false;
			}
		}
	}
	//ランダム最低値
	const float minShakeValue = -0.7f;
	//ランダム最高値
	const float maxShakeValue = 0.7f;
	//有効化フラグ
	bool flag = false;
	//止める時間
	float timer = 0.0f;
	//現在のTの値
	float easeT = 0.0f;
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

	static void SetFOV(float FOVvalue);
	static void SetShake(float Time);

	static WorkInterpolation workInter;
	static WorkFOV workFOV;
	static WorkShake workShake;

	void ImGui();
#pragma region
	void Shake();
	static bool isShake;
	void SetDelayFlag(bool flag) {
		DelayFlag = flag;
	};
#pragma endregion シェイク
private:

	float interParameter_ = 1.0f;
	bool DelayFlag = false;
	Vector3 prePos_;
	Vector3 currentPos_;

	WorkLockAt workLockAt;

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

};
