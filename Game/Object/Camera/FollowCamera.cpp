#include "FollowCamera.h"

void FollowCamera::Initialize() {
	viewProj.Initialize();
	workInter.interParameter_ = 1.0f;
}

void FollowCamera::Update() {

	//スティックでのカメラ回転
	if (Input::GetInstance()->GetJoystickState(joyState)) {

		const float kRadian = 0.03f;

		rotate_.y += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * kRadian;
		rotate_.x -= (float)joyState.Gamepad.sThumbRY / SHRT_MAX * kRadian;


		//カメラの上下移動を制御
		if (rotate_.x > 1.0f) {
			rotate_.x = 1.0f;
		}
		else if (rotate_.x < -1.0f) {
			rotate_.x = -1.0f;
		}

		parameter_t = 1.0f;
	}		
	Vector3 EulerRot;
	//TODO : LerpShortAngleを使うと一定角度で急にワープする
	/*EulerRot.z = LerpShortAngle(viewProj.rotation_.z, rotate_.z, parameter_t);
	EulerRot.y = LerpShortAngle(viewProj.rotation_.y, rotate_.x, parameter_t);
	EulerRot.x = LerpShortAngle(viewProj.rotation_.x, rotate_.y, parameter_t);*/
	EulerRot.z = rotate_.z;
	EulerRot.y = rotate_.y;
	EulerRot.x = rotate_.x;
	viewProj.rotation_ = Quaternion::EulerToQuaterion(EulerRot);

	if (target_) {
		Vector3 pos = target_->transform_.translate;
		//もしペアレントを結んでいるなら
		if (target_->parent_) {
			pos = target_->transform_.translate + target_->parent_->transform_.translate;
		}
		//追従座標の補間
		//workInter.interTarget_ = Vector3::VectorLerp(workInter.interTarget_, pos, workInter.interParameter_);

		Vector3 offset = OffsetCalc();
		//オフセット分と追従座標の補間分ずらす
		viewProj.translation_ = pos + offset;
	}

	viewProj.UpdateMatrix();
}

void FollowCamera::SetTarget(const WorldTransform* target)
{
	target_ = target;
	Reset();
}

void FollowCamera::Reset()
{
	//追従対象がいれば
	if (target_) {
		//追従座標・角度の初期化
		workInter.interTarget_ = target_->transform_.translate;
		viewProj.rotation_.y = target_->transform_.quaternion.y;
	}
	workInter.targetAngleY_ = viewProj.rotation_.y;

	//追従大賞からのオフセット
	Vector3 offset = OffsetCalc();
	viewProj.translation_ = workInter.interTarget_ + offset;
}

Vector3 FollowCamera::OffsetCalc()
{
	Vector3 offset = offsetPos;
	//回転行列の合成
	Matrix4x4 rotateMatrix = MakeRotateMatrix(viewProj.rotation_);

	// オフセットをカメラの回転に合わせて回転
	offset = TransformNormal(offset, rotateMatrix);

	return offset;
}