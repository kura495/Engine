#include "DebugCamera.h"

void DebugCamera::Initialize()
{
	viewProj.Initialize();
	viewProj.translation_ = {0.0f,0.0f,-10.0f};
	viewProj.UpdateMatrix();
}

void DebugCamera::Update()
{
	if (Input::GetInstance()->TriggerKey(DIK_LALT)) {
#pragma region rotation
	const float rotSpeed = 0.05f;

	if (Input::GetInstance()->TriggerKey(DIK_UP)) {
		rotate_.x -= rotSpeed;
	}
	else if (Input::GetInstance()->TriggerKey(DIK_DOWN)) {
		rotate_.x += rotSpeed;
	}
	if (Input::GetInstance()->TriggerKey(DIK_LEFT)) {
		rotate_.y -= rotSpeed;
	}
	else if (Input::GetInstance()->TriggerKey(DIK_RIGHT)) {
		rotate_.y += rotSpeed;
	}
	Vector3 EulerRot;
	EulerRot.x = LerpShortAngle(viewProj.rotation_.x, rotate_.x, 1.0f);
	EulerRot.y = LerpShortAngle(viewProj.rotation_.y, rotate_.y, 1.0f);
	EulerRot.z = LerpShortAngle(viewProj.rotation_.z, rotate_.z, 1.0f);

	viewProj.rotation_ = Quaternion::EulerToQuaterion(EulerRot);
#pragma endregion 回転
#pragma region translation_
	const float translateSpeed = 0.5f;
	if (Input::GetInstance()->TriggerKey(DIK_A)) {
		viewProj.translation_.x -= translateSpeed;
	}
	else if (Input::GetInstance()->TriggerKey(DIK_D)) {
		viewProj.translation_.x += translateSpeed;
	}
	if (Input::GetInstance()->TriggerKey(DIK_W)) {
		viewProj.translation_.y += translateSpeed;
	}
	else if (Input::GetInstance()->TriggerKey(DIK_S)) {
		viewProj.translation_.y -= translateSpeed;
	}
	if (Input::GetInstance()->TriggerKey(DIK_E)) {
		viewProj.translation_.z -= translateSpeed;
	}
	else if (Input::GetInstance()->TriggerKey(DIK_Q)) {
		viewProj.translation_.z += translateSpeed;
	}
#pragma endregion 移動
	}
	viewProj.UpdateMatrix();

}