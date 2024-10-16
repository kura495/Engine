#pragma once
#include "Camera/Camera.h"
#include "Input/Input.h"
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
	float interParameter_ = 0.5f;
};

class FollowCamera : public Camera
{
public:
	void Initialize()override;
	void Update()override;
	void ImGui();
	void SetTarget(const WorldTransform* target);

	bool PlaySceneInit(const WorldTransform* target);
	void PlaySceneReset();
private:
#pragma region
	Vector3 InitCameraPos = {-1.4f,0.4f,-2.3f};
	Vector3 InitCameraRot = {-0.3f,0.1f,0.0f};
	Vector3 InitCameraPos2 = {-1.5f,1.9f,1.9f};
	Vector3 InitCameraRot2 = {0.5f,2.6f,0.0f};

	bool resetFlag_ = true;
	float lerpTTitle = 0.0f;
	float addValueTitle = 0.01f;
	Vector3 resetTransform;
	Quaternion resetRotate;
#pragma endregion PlaySceneInit用

	Vector3 chackTargetPos;
	Vector3 PrePos;

	Vector3 EulerRot;
	//追従対象
	const WorldTransform* target_ = nullptr;
	// ゲームパッド
	XINPUT_STATE joyState;

	Vector3 rotate_ = { 0.0f,0.0f,0.0f };
	float rotAngle_;

	float parameter_t = 0.0f;
	float GrapParameter_t = 0.0f;

	//追従対象の座標・角度を再設定
	void Reset();
	WorkInterpolation workInter;
	//追従対象からのオフセットを計算する
	Vector3 OffsetCalc();

	Vector3 offsetPos = { 0.0f, 2.0f, -15.0f };
};
