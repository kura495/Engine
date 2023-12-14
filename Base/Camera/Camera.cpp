#include"Camera.h"


void Camera::Initialize()
{
	input = Input::GetInstance();
	viewProj.Initialize();
}

void Camera::Update()
{
#ifdef _DEBUG
	if (DebucCameraFlag) {
		DebugCameraMove();
	}
#endif // _DEBUG
	viewProj.UpdateMatrix();
}

#ifdef _DEBUG
void Camera::DebugCameraMove()
{
#pragma region rotation
	const float rotSpeed = 0.05f;
	if (input->TriggerKey(DIK_UP)) {
		viewProj.rotation_.x += rotSpeed;
	}
	else if (input->TriggerKey(DIK_DOWN)) {
		viewProj.rotation_.x -= rotSpeed;
	}
	if (input->TriggerKey(DIK_LEFT)) {
		viewProj.rotation_.y -= rotSpeed;
	}
	else if (input->TriggerKey(DIK_RIGHT)) {
		viewProj.rotation_.y += rotSpeed;
	}
		
#pragma endregion 回転
#pragma region translation_
	const float translateSpeed = 0.5f;
	if (input->TriggerKey(DIK_A)) {
		viewProj.translation_.x -= translateSpeed;
	}
	else if (input->TriggerKey(DIK_D)) {
		viewProj.translation_.x += translateSpeed;
	}
	if (input->TriggerKey(DIK_W)) {
		viewProj.translation_.y += translateSpeed;
	}
	else if (input->TriggerKey(DIK_S)) {
		viewProj.translation_.y -= translateSpeed;
	}
	if (input->TriggerKey(DIK_E)) {
		viewProj.translation_.z -= translateSpeed;
	}
	else if (input->TriggerKey(DIK_Q)) {
		viewProj.translation_.z += translateSpeed;
	}
#pragma endregion 移動
}
#endif // DEBUG