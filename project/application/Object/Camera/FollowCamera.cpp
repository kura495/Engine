#include "FollowCamera.h"

WorkInterpolation FollowCamera::workInter;

void FollowCamera::Initialize() {
	viewProj.Initialize();

	workInter.interTarget_ = { 0.0f,0.0f,0.0f };
	PlaySceneReset();
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

	EulerRot.x = rotate_.x;
	EulerRot.y = rotate_.y;
	EulerRot.z = rotate_.z;
	viewProj.rotation_ = Quaternion::EulerToQuaterion(EulerRot);

	if (target_) {
		Vector3 pos = target_->transform.translate;
		//追従座標の補間
		workInter.interTarget_.x = Vector3::Lerp(workInter.interTarget_, pos, workInter.interParameter_.x).x;
		workInter.interTarget_.z = Vector3::Lerp(workInter.interTarget_, pos, workInter.interParameter_.z).z;

		workInter.interTarget_.y = Vector3::Lerp(workInter.interTarget_, pos, workInter.interParameter_.y).y;

		Vector3 offset = OffsetCalc();
		//オフセット分と追従座標の補間分ずらす
		viewProj.translation_ = workInter.interTarget_ + offset;
	}

	//画面を揺らす
	if (isShake) {
		Shake();
	}

	viewProj.Update();
}

void FollowCamera::Shake()
{
	//ランダム生成用
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());
	std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
	Vector3 ramdomTranslate = { distribution(randomEngine),distribution(randomEngine) ,distribution(randomEngine) };

	viewProj.translation_.x += ramdomTranslate.x;
	viewProj.translation_.y += ramdomTranslate.y;
	viewProj.translation_.z += ramdomTranslate.z;
}

void FollowCamera::ImGui()
{
#ifdef _DEBUG
	ImGui::Begin("FollowCamera");
	ImGui::DragFloat3("Offset", &offsetPos.x);
	ImGui::DragFloat3("translate", &InitCameraPos2.x, 0.1f);
	ImGui::DragFloat3("rotate", &InitCameraRot2.x, 0.1f);
	if (!target_) {
		viewProj.translation_ = InitCameraPos2;
		viewProj.rotation_ = Quaternion::EulerToQuaterion(InitCameraRot2);
	}
	if (ImGui::Button("isShakeOn")) {
		isShake = true;
	}
	if (ImGui::Button("isShakeOff")) {
		isShake = false;
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
		viewProj.rotation_.y = LerpShortAngle(viewProj.rotation_.y, target_->transform.quaternion.y, 1.0f);
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