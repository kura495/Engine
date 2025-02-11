#include "DebugCamera.h"

void DebugCamera::Initialize()
{
	parameter.translation_ = {0.0f,0.0f,-10.0f};
}

void DebugCamera::Update()
{
	//LALTを押している
	if (Input::GetInstance()->TriggerKey(DIK_LALT)) {
	//キー入力でカメラ回転
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
	EulerRot.x = LerpShortAngle(parameter.rotation_.x, rotate_.x, 1.0f);
	EulerRot.y = LerpShortAngle(parameter.rotation_.y, rotate_.y, 1.0f);
	EulerRot.z = LerpShortAngle(parameter.rotation_.z, rotate_.z, 1.0f);

	parameter.rotation_ = Quaternion::EulerToQuaterion(EulerRot);
#pragma endregion 回転
	//キー入力でカメラ移動
#pragma region translation_
	const float translateSpeed = 0.5f;
	if (Input::GetInstance()->TriggerKey(DIK_A)) {
		parameter.translation_.x -= translateSpeed;
	}
	else if (Input::GetInstance()->TriggerKey(DIK_D)) {
		parameter.translation_.x += translateSpeed;
	}
	if (Input::GetInstance()->TriggerKey(DIK_W)) {
		parameter.translation_.y += translateSpeed;
	}
	else if (Input::GetInstance()->TriggerKey(DIK_S)) {
		parameter.translation_.y -= translateSpeed;
	}
	if (Input::GetInstance()->TriggerKey(DIK_E)) {
		parameter.translation_.z -= translateSpeed;
	}
	else if (Input::GetInstance()->TriggerKey(DIK_Q)) {
		parameter.translation_.z += translateSpeed;
	}
#pragma endregion 移動
	}
}