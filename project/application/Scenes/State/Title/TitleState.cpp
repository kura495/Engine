#include"Scenes/State/Title/TitleState.h"

void TitleState::Init()
{
	StateNo = GameStateNo::TITLE;


	//天球
	skyDome_ = std::make_unique<SkyDome>();
	skyDome_->Init();

	ufo_ = std::make_unique<UFO>();
	ufo_->Init();

}

void TitleState::Update()
{
	if (0 < Input::GetPadTrreger().x && 0 < Input::GetPadTrreger().y) {
		ImGui::Begin("Title");
		ImGui::Text("Push");
		ImGui::End();
		int vibValue = (int)Input::GetPadTrreger().x;
		Input::VibrateController(vibValue * 10, vibValue * 10);
	}
	ImGui::Begin("Title");
	Vector2 Test = Input::GetPadTrreger();
	ImGui::Text("%f",Test.x);
	ImGui::Text("%f",Test.y);
	ImGui::End();

	skyDome_->Update();
	ufo_->Update();
}

void TitleState::Draw()
{
	skyDome_->Draw();
	ufo_->Draw();
}
