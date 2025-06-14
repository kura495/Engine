#include "FollowCamera.h"

WorkInterpolation FollowCamera::workInter;
WorkFOV FollowCamera::workFOV;

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

	//画面を揺らす
	if (isShake) {
		Shake();
	}
}

void FollowCamera::Shake()
{
	Vector3 ramdomTranslate = { random::Generate<float>(minShakeValue,maxShakeValue),random::Generate<float>(minShakeValue,maxShakeValue),random::Generate<float>(minShakeValue, maxShakeValue) };

	parameter.translation_ += ramdomTranslate;
}

void FollowCamera::SetFOV(float FOVvalue)
{
	workFOV.easeT = 0.0f;
	workFOV.newFOVvalue = FOVvalue;
	workFOV.flag = true;
}

void FollowCamera::ImGui()
{
#ifdef _DEBUG
	ImGui::Begin("FollowCamera");
	ImGui::DragFloat3("Offset", &offsetPos.x);
	if (ImGui::Button("isShakeOn")) {
		isShake = true;
	}
	if (ImGui::Button("isShakeOff")) {
		isShake = false;
	}
	ImGui::End();
#endif
}

void FollowCamera::ShakeFanction()
{
	Vector3 ramdomTranslate = { random::Generate<float>(-1.0f, 1.0f),random::Generate<float>(-1.0f, 1.0f) ,random::Generate<float>(-1.0f, 1.0f) };

	parameter.translation_ += ramdomTranslate;
}

void FollowCamera::SetTarget(const WorldTransform* target)
{
	target_ = target;
	Reset();
}
void FollowCamera::LockAt(const WorldTransform& target)
{
	Vector3 lockVector = { 0.0f,0.0f,0.0f };
	workLockAt.easeT = 0.0f;
	if (target_) {
		lockVector = target.transform.translate - target_->transform.translate;
		lockVector = lockVector.Normalize();
		if (lockVector.z != 0.0) {
			workLockAt.lockAtRat = std::asin(lockVector.x / std::sqrt(lockVector.x * lockVector.x + lockVector.z * lockVector.z));

			if (lockVector.z < 0.0) {
				workLockAt.lockAtRat = (lockVector.x >= 0.0) ? std::numbers::pi_v<float> -workLockAt.lockAtRat : -std::numbers::pi_v<float> -workLockAt.lockAtRat;
			}
		}
		else {
			workLockAt.lockAtRat = (lockVector.x >= 0.0) ? std::numbers::pi_v<float> / 2.0f : -std::numbers::pi_v<float> / 2.0f;
		}
	}
	workLockAt.flag = true;
}
void FollowCamera::ReStert()
{
	rotate_ = { 0.0f,0.0f,0.0f };
	//回転をしていない状態にリセット
	parameter.rotation_ = Quaternion::IdentityQuaternion();
	//ターゲットがあるなら補間をなくす
	if (target_) {
		prePos_ = currentPos_;

		currentPos_ = target_->transform.translate;
		
		workInter.interTarget_ = currentPos_;

		Vector3 offset = OffsetCalc();
		//オフセット分と追従座標の補間分ずらす
		parameter.translation_ = workInter.interTarget_ + offset;
	}
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