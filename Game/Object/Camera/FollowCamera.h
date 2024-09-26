#pragma once
#include "Base/Camera/Camera.h"
#include "Base/Input/Input.h"
#include "WorldTransform/WorldTransform.h"
#include "Math/Matrix/MatrixCalc.h"
#include "Math/Vector/VectorCalc.h"
#include "Calc.h"

//カメラ補間用ワーク
struct WorkInterpolation {
	//追従対象の残像座標
	Vector3 interTarget_ = {};
	//追従対象のY軸
	float targetAngleY_ = 0.0f;
	//カメラ補間の媒介変数
	float interParameter_ = 1.0f;
};

class FollowCamera : public Camera
{
public:
	void Initialize()override;
	void Update()override;
	void ImGui();
	void SetTarget(const WorldTransform* target);

private:
	//追従対象
	const WorldTransform* target_ = nullptr;
	// ゲームパッド
	XINPUT_STATE joyState;

	Vector3 rotate_ = { 0.0f,0.0f,0.0f };

	float parameter_t = 0.0f;
	float GrapParameter_t = 0.0f;

	//追従対象の座標・角度を再設定
	void Reset();
	WorkInterpolation workInter;
	//追従対象からのオフセットを計算する
	Vector3 OffsetCalc();

	Vector3 offsetPos = { 0.0f, 2.0f, -15.0f };
};
