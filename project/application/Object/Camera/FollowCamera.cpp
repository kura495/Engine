#include "FollowCamera.h"

WorkInterpolation FollowCamera::workInter;
WorkFOV FollowCamera::workFOV;
WorkShake FollowCamera::workShake;

void FollowCamera::Initialize() {
	workInter.interTarget_ = { 0.0f,0.0f,0.0f };
	workInter.interParameter_.x = 0.5f;
	workInter.interParameter_.y = 0.5f;
	workInter.interParameter_.z = 0.5f;
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
	}
	Vector3 EulerRot;
	//LockAtの更新処理
	workLockAt.Update(rotate_.y);
	//FOVの変更処理
	workFOV.Update(parameter.FOV);

	EulerRot.x = rotate_.x;
	EulerRot.y = rotate_.y;
	EulerRot.z = rotate_.z;

	parameter.rotation_ = Quaternion::EulerToQuaterion(EulerRot);

	if (target_) {
		prePos_ = currentPos_;

		currentPos_ = target_->transform.translate;
		if (prePos_.x != currentPos_.x || prePos_.y != currentPos_.y || prePos_.z != currentPos_.z) {
			DelayFlag = true;
		}
		if (DelayFlag) {
			interParameter_ = 0.3f;
		}
		else {
			interParameter_ = (std::min)(interParameter_ + workInter.addeaseT, 1.0f);
		}
		//追従座標の補間
		workInter.interTarget_ = Vector3::Lerp(workInter.interTarget_, currentPos_, interParameter_);


		Vector3 offset = OffsetCalc();
		//オフセット分と追従座標の補間分ずらす
		parameter.translation_ = workInter.interTarget_ + offset;
	}

	//カメラのシェイク処理
	workShake.Update(parameter.translation_);

}
void FollowCamera::SetFOV(float FOVvalue)
{
	workFOV.Init(FOVvalue);
}
void FollowCamera::SetShake(float Time,Vector2 ShakeValue)
{
	workShake.Init(Time,ShakeValue);
}
void FollowCamera::ImGui()
{
#ifdef _DEBUG
	ImGui::Begin("FollowCamera");
	ImGui::DragFloat3("Offset", &offsetPos.x);
	ImGui::End();
#endif
}
void FollowCamera::SetTarget(const WorldTransform* target)
{
	target_ = target;
	Reset();
}
void FollowCamera::LockAt(const WorldTransform& target)
{
	workLockAt.Init(target_->transform.translate,target);
}
void FollowCamera::ReStert()
{
	rotate_ = { 0.0f,0.0f,0.0f };
	//回転をしていない状態にリセット
	parameter.rotation_ = Quaternion::IdentityQuaternion();
}
void FollowCamera::Reset()
{
	//追従対象がいれば
	if (target_) {
		//追従座標・角度の初期化
		workInter.interTarget_ = target_->transform.translate;
		parameter.rotation_.y = LerpShortAngle(parameter.rotation_.y, target_->transform.quaternion.y, 1.0f);
	}
	workInter.targetAngleY_ = parameter.rotation_.y;

	//追従大賞からのオフセット
	Vector3 offset = OffsetCalc();
	parameter.translation_ = workInter.interTarget_ + offset;
}

Vector3 FollowCamera::OffsetCalc()
{
	Vector3 offset = offsetPos;
	//回転行列の合成
	Matrix4x4 rotateMatrix = MakeRotateMatrix(parameter.rotation_);

	// オフセットをカメラの回転に合わせて回転
	offset = TransformNormal(offset, rotateMatrix);

	return offset;
}