#include "DebugCamera.h"

void DebugCamera::Initialize()
{
	viewProj.Initialize();
	viewProj.translation_ = {0.0f,2.0f,-25.0f};
	viewProj.UpdateMatrix();
}

void DebugCamera::Update()
{
	ImGui();
	if (Input::GetInstance()->TriggerKey(DIK_LALT)) {
#pragma region rotation
	const float rotSpeed = 0.05f;
	if (Input::GetInstance()->TriggerKey(DIK_UP)) {
		viewProj.rotation_.x += rotSpeed;
	}
	else if (Input::GetInstance()->TriggerKey(DIK_DOWN)) {
		viewProj.rotation_.x -= rotSpeed;
	}
	if (Input::GetInstance()->TriggerKey(DIK_LEFT)) {
		viewProj.rotation_.y -= rotSpeed;
	}
	else if (Input::GetInstance()->TriggerKey(DIK_RIGHT)) {
		viewProj.rotation_.y += rotSpeed;
	}

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

void DebugCamera::ImGui()
{
#ifdef _DEBUG
	ImGui::Begin("Camera");
	ImGui::DragFloat3("Rotate",&viewProj.rotation_.x);
	ImGui::DragFloat3("Translate",&viewProj.translation_.x);
	if (ImGui::Button("Reset")) {
		viewProj.translation_ = { 0.0f,0.0f,0.0f };
		viewProj.UpdateMatrix();
	}
	ImGui::End();
#endif
}
