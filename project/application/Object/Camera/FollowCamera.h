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

#include "Work/FOV/WorkFOV.h"
#include "Work/LockAt/WorkLockAt.h"
#include "Work/Shake/WorkShake.h"

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
	/// FOVを調節する機能
	/// </summary>
	/// <param name="FOVvalue">視野角の広さ</param>
	static void SetFOV(float FOVvalue);
	/// <summary>
	/// カメラを揺らす演出
	/// </summary>
	/// <param name="Time">何秒揺らすか</param>
	/// <param name="ShakeValue">揺れの幅　x = min y = max</param>
	static void SetShake(float Time, Vector2 ShakeValue = { -0.7f,0.7f });
	/// <summary>
	/// 
	/// </summary>
	/// <param name="target">対象のWorldTransform</param>
	void SetLockAt(const WorldTransform& target);

	void ReStert();

	void SetOffset(Vector3 offset) { offsetPos = offset; };


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
	WorkLockAt workLockAt;

	float interParameter_ = 1.0f;
	bool DelayFlag = false;
	Vector3 prePos_;
	Vector3 currentPos_;

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
