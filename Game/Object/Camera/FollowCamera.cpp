#include "FollowCamera.h"

void FollowCamera::Initalize() {
	viewProjection_.Initialize();
	workInter.interParameter_ = 1.0f;
}

void FollowCamera::Update() {

	if (target_) {
		Vector3 pos = target_->transform_.translate;
		//もしペアレントを結んでいるなら
		if (target_->parent_) {
			pos = Add(target_->transform_.translate, target_->parent_->transform_.translate);
		}
		//追従座標の補間
		workInter.interTarget_ = VectorLerp(workInter.interTarget_, pos, workInter.interParameter_);

		Vector3 offset = OffsetCalc();
		//オフセット分と追従座標の補間分ずらす
		viewProjection_.translation_ = workInter.interTarget_ + offset;
	}

	//スティックでのカメラ回転
	if (Input::GetInstance()->GetJoystickState(joyState)) {

		const float kRadian = 0.02f;

		rotate_.y -= (float)joyState.Gamepad.sThumbRY / SHRT_MAX * kRadian;
		rotate_.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * kRadian;
		if (rotate_.y > 1.0f) {
			rotate_.y = 1.0f;
		}
		else if (rotate_.y < -1.0f) {
			rotate_.y = -1.0f;
		}


		parameter_t = 1.0f;
	}
	viewProjection_.rotation_.y = LerpShortAngle(viewProjection_.rotation_.y, rotate_.x, parameter_t);
	viewProjection_.rotation_.x = LerpShortAngle(viewProjection_.rotation_.x, rotate_.y, parameter_t);
	viewProjection_.UpdateMatrix();
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
		viewProjection_.rotation_.y = target_->transform_.quaternion.y;
	}
	workInter.targetAngleY_ = viewProjection_.rotation_.y;

	//追従大賞からのオフセット
	Vector3 offset = OffsetCalc();
	viewProjection_.translation_ = workInter.interTarget_ + offset;
}

Vector3 FollowCamera::OffsetCalc()
{
	Vector3 offset = { 0.0f, 2.0f, 0.0f };
	//回転行列の合成
	Matrix4x4 rotateMatrix = MakeRotateMatrix(viewProjection_.rotation_);

	// オフセットをカメラの回転に合わせて回転
	offset = TransformNormal(offset, rotateMatrix);

	return offset;
}