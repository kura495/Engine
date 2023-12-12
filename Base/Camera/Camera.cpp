#include"Camera.h"


void Camera::Initialize()
{
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
	const float speed = 0.5f;
	if (input->TriggerKey(DIK_UP)) {
		viewProj.rotation_.x += speed;
	}
	else if (input->TriggerKey(DIK_DOWN)) {
		viewProj.rotation_.x -= speed;
	}
	if (input->TriggerKey(DIK_LEFT)) {
		viewProj.rotation_.y -= speed;
	}
	else if (input->TriggerKey(DIK_RIGHT)) {
		viewProj.rotation_.y += speed;
	}
		
#pragma endregion 回転
#pragma region translation_

	if (input->TriggerKey(DIK_A)) {
		viewProj.translation_.x -= speed;
	}
	else if (input->TriggerKey(DIK_D)) {
		viewProj.translation_.x += speed;
	}
	if (input->TriggerKey(DIK_W)) {
		viewProj.translation_.y += speed;
	}
	else if (input->TriggerKey(DIK_S)) {
		viewProj.translation_.y -= speed;
	}
	if (input->TriggerKey(DIK_E)) {
		viewProj.translation_.z -= speed;
	}
	else if (input->TriggerKey(DIK_Q)) {
		viewProj.translation_.z += speed;
	}
#pragma endregion 移動
}
#endif // DEBUG