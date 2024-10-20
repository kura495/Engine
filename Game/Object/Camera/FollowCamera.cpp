#include "FollowCamera.h"

void FollowCamera::Initialize() {
	viewProj.Initialize();
	workInter.interParameter_ = 1.0f;

	viewProj.translation_ = InitCameraPos2;
	viewProj.rotation_ = Quaternion::EulerToQuaterion(InitCameraRot2);
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

	//TODO : LerpShortAngleを使うと一定角度で急にワープする
	if (resetFlag_ == false) {
		EulerRot.x = rotate_.x;
		EulerRot.y = rotate_.y;
		EulerRot.z = rotate_.z;
		viewProj.rotation_ = Quaternion::EulerToQuaterion(EulerRot);

		if (target_) {
			Vector3 pos = target_->transform.translate;
			//追従座標の補間
			workInter.interTarget_ = Vector3:: Lerp(viewProj.translation_, pos, workInter.interParameter_);

			Vector3 offset = OffsetCalc();
			//オフセット分と追従座標の補間分ずらす
			viewProj.translation_ = workInter.interTarget_ + offset;

			////もしペアレントを結んでいるなら
			//if (target_->parent_) {
			//	pos = target_->transform.translate + target_->parent_->transform.translate;
			//}
			////追従座標の補間
			//workInter.interTarget_ = Vector3::Lerp(target_->transform.translate, pos, workInter.interParameter_);

			//Vector3 offset = OffsetCalc();
			////オフセット分と追従座標の補間分ずらす
			//viewProj.translation_ = pos + offset;
		}
	}
#ifdef _DEBUG
	ImGui();
#endif

	viewProj.Update();
}

void FollowCamera::ImGui()
{
#ifdef _DEBUG
	ImGui::Begin("FollowCamera");
	ImGui::DragFloat3("Offset",&offsetPos.x);
	ImGui::DragFloat3("translate",&InitCameraPos2.x,0.1f);
	ImGui::DragFloat3("rotate",&InitCameraRot2.x,0.1f);
	if (!target_) {
		viewProj.translation_ = InitCameraPos2;
		viewProj.rotation_ = Quaternion::EulerToQuaterion(InitCameraRot2);
	}

	ImGui::End();
#endif
}

void FollowCamera::SetTarget(const WorldTransform* target)
{
	target_ = target;
	Reset();
}

bool FollowCamera::PlaySceneInit(const WorldTransform* target)
{
	if (lerpTTitle == 0.0f) {
		resetTransform = viewProj.translation_;
		resetRotate = viewProj.rotation_;
		rotate_ = { 0.0f,0.0f,0.0f };
		target_ = target;
		resetFlag_ = true;
	}

	PlaySceneReset();
	lerpTTitle += addValueTitle;

	if (lerpTTitle > 1.0f) {
		resetFlag_ = false;
		return true;
	}
	return false;
}

void FollowCamera::PlaySceneReset()
{
	//追従対象がいれば
	if (target_) {
		//追従座標・角度の初期化
		workInter.interTarget_ = Vector3::Lerp(resetTransform, target_->transform.translate + offsetPos, lerpTTitle);
		viewProj.rotation_ = Quaternion::Slerp(resetRotate, target_->transform.quaternion, lerpTTitle);
	}

	//追従大賞からのオフセット
	Vector3 offset = OffsetCalc();
	viewProj.translation_ = workInter.interTarget_;
}

void FollowCamera::Reset()
{
	//追従対象がいれば
	if (target_) {
		//追従座標・角度の初期化
		workInter.interTarget_ = target_->transform.translate;
		viewProj.rotation_.y = LerpShortAngle(viewProj.rotation_.y,target_->transform.quaternion.y, 1.0f);
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