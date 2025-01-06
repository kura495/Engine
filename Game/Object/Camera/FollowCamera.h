#pragma once
//ターゲットを追従するカメラ//
#include <random>
#include <numbers>

#include "Camera/Camera.h"
#include "Input/Input.h"
#include "WorldTransform/WorldTransform.h"
#include "Math/Matrix/MatrixCalc.h"
#include "Calc.h"

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
	/// ゲームプレイシーンがプレイビヘイビアーに移行するときに使用
	/// </summary>
	/// <param name="target">ターゲットのWorldTransform</param>
	bool PlaySceneInit(const WorldTransform* target);
	void PlaySceneReset();

	static WorkInterpolation workInter;

	
#pragma region
	void Shake();
	bool isShake = false;
#pragma endregion シェイク
private:
	void ImGui();
#pragma region
	//カメラの位置と向き
	Vector3 InitCameraPos2 = {-1.5f,1.9f,1.9f};
	Vector3 InitCameraRot2 = {0.5f,2.6f,0.0f};

	bool resetFlag_ = true;
	float lerpTTitle = 0.0f;
	float addValueTitle = 0.01f;
	Vector3 resetTransform;
	Quaternion resetRotate;
#pragma endregion PlaySceneInit用



	//追従対象
	const WorldTransform* target_ = nullptr;
	// ゲームパッド
	XINPUT_STATE joyState;

	Vector3 rotate_ = { 0.0f,0.0f,0.0f };

	//追従対象の座標・角度を再設定
	void Reset();

	//追従対象からのオフセットを計算する
	Vector3 OffsetCalc();

	Vector3 offsetPos = { 0.0f, 3.0f, -10.0f };
};
