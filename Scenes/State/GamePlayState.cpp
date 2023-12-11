#include "Scenes/State/GamePlayState.h"

void GamePlayState::Initialize()
{
	//基本機能生成
	camera_ = new Camera();
	camera_->Initialize(1280, 720);
	input = Input::GetInstance();
	audio = Audio::GetInstance();
	textureManager_ = TextureManager::GetInstance();
	light_ = Light::GetInstance();

	DirectX_ = DirectXCommon::GetInstance();

	//
	//3Dオブジェクト生成


}

void GamePlayState::Update()
{
//デバッグカメラ
#ifdef _DEBUG
if (input->TriggerKey(DIK_LALT)) {
		camera_->DebugCamera(true);
}
else {
	camera_->DebugCamera(false);
}
#endif // _DEBUG
#ifdef _DEBUG

m_pos = input->GetMousePosition();

ImGui::Begin("PlayState");
ImGui::Text("PosX : %f",m_pos.Pos.x);
ImGui::Text("PosY : %f",m_pos.Pos.y);
ImGui::Text("Scroll : %f",m_pos.Scroll);

if (input->pushMouse(MOUSE_BOTTON0)) {
	ImGui::Text("A");
}
if (input->TriggerPad(XINPUT_GAMEPAD_B)) {
	ImGui::Text("B");
}
if (input->TriggerPad(XINPUT_GAMEPAD_X)) {
	ImGui::Text("X");
}
if (input->TriggerPad(XINPUT_GAMEPAD_Y)) {
	ImGui::Text("Y");
}
if (input->TriggerPad(XINPUT_GAMEPAD_START)) {
	ImGui::Text("START");
}
if (input->TriggerPad(XINPUT_GAMEPAD_LEFT_SHOULDER)) {
	ImGui::Text("LEFT");
}
if (input->TriggerPad(XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
	ImGui::Text("RIGHT");
}
if (input->TriggerPad(XINPUT_GAMEPAD_LEFT_THUMB)) {
	ImGui::Text("LEFT_THUMB");
}if (input->TriggerPad(XINPUT_GAMEPAD_RIGHT_THUMB)) {
	ImGui::Text("RIGHT_THUMB");
}
ImGui::End();
#endif



}

void GamePlayState::Draw()
{
	//3Dモデル描画ここから

	//3Dモデル描画ここまで	


	//Sprite描画ここから


	//Sprite描画ここまで
	
	//描画ここまで
}

