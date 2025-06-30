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
	Vector3 interParameter_ = { 1.0f,1.0f,1.0f };
	//線形補間の1フレームごとの加算値
	float addeaseT = 0.0002f;

};
/// <summary>
/// work基底クラス
/// </summary>
class Iwork {
protected:
	//有効化フラグ
	bool flag = false;
	//現在のTの値
	float easeT = 0.0f;
	//raseTに毎フレーム加算する値
	float AddEaseT = 0.02f;
};
//ベクトル方向に向ける処理クラス
class WorkLockAt : public Iwork {
public:
	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="axis">回転の中心軸の位置</param>
	/// <param name="target">向きたい物</param>
	void Init(const Vector3& axis, const WorldTransform& target) {
		Vector3 lockVector = { 0.0f,0.0f,0.0f };
		easeT = 0.0f;
		flag = true;

		lockVector = target.transform.translate - axis;
		lockVector = lockVector.Normalize();
		if (lockVector.z != 0.0) {
			//asinでsinから角度を計算
			lockAtRat = std::asin(lockVector.x / std::sqrt(lockVector.x * lockVector.x + lockVector.z * lockVector.z));
			//zが手前に向いている
			if (lockVector.z < 0.0) {
				//xが0より大きいかどうか
				//大きい = 円の右下
				//小さい = 円の左下
				lockAtRat = (lockVector.x >= 0.0) ? std::numbers::pi_v<float> -lockAtRat : -std::numbers::pi_v<float> -lockAtRat;
			}
		}
		//zが真上を向いている場合
		else {
			lockAtRat = (lockVector.x >= 0.0) ? std::numbers::pi_v<float> / 2.0f : -std::numbers::pi_v<float> / 2.0f;
		}
	}
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
private:
	//回転
	float lockAtRat = 0.0f;
};
class WorkFOV : public Iwork {
public:
	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="FOVvalue">視野角の値</param>
	void Init(float FOVvalue) {
		easeT = 0.0f;
		flag = true;
		newFOVvalue = FOVvalue;
	}
	/// <summary>
	/// FOVを線形補間しながら変更
	/// </summary>
	/// <param name="FOV">変更するFOVの値</param>
	void Update(float& FOV) {
		if (flag) {
			easeT = (std::min)(easeT + AddEaseT, 1.0f);
			FOV = Lerp(FOV, newFOVvalue, easeT);
			if (easeT == 1.0f) {
				flag = false;
			}
		}
	}
private:
	//変更する値
	float newFOVvalue = 0.0f;
};
class WorkShake : public Iwork {
public:
	/// <summary>
	/// 初期化処理
	/// 揺らす時間と大きさを設定
	/// </summary>
	/// <param name="Time">有効時間</param>
	/// <param name="ShakeValue">揺れの大きさ x = 最小値 y = 最大値</param>
	void Init(float Time, Vector2 ShakeValue) {
		easeT = 0.0f;
		flag = true;
		timer = Time;
		minShakeValue = ShakeValue.x;
		maxShakeValue = ShakeValue.y;
	}
	/// <summary>
	/// 揺らす処理
	/// </summary>
	/// <param name="translate">揺らす物の座標</param>
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
private:
	//ランダム最低値
	float minShakeValue = -0.7f;
	//ランダム最高値
	float maxShakeValue = 0.7f;
	//止める時間
	float timer = 0.0f;
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
	/// <summary>
	/// カメラを揺らす演出
	/// </summary>
	/// <param name="Time">何秒揺らすか</param>
	/// <param name="ShakeValue">揺れの幅　x = min y = max</param>
	static void SetShake(float Time, Vector2 ShakeValue = { -0.7f,0.7f });

	void ImGui();
#pragma region
	void SetDelayFlag(bool flag) {
		DelayFlag = flag;
	};
#pragma endregion シェイク
private:

	static WorkInterpolation workInter;
	static WorkFOV workFOV;
	static WorkShake workShake;

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

	Vector3 offsetPos = { 0.0f,0.0f,0.0f };

};
