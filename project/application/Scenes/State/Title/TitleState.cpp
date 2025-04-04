#include"Scenes/State/Title/TitleState.h"

void TitleState::Init()
{
	StateNo = GameStateNo::TITLE;

	UFOmodel_ = std::make_unique<Model>();
	UFOmodel_.reset(Model::CreateModelFromObj("project/resources/UFO", "UFO.obj"));

	UFOWorld_.Init();
	UFOWorld_.transform.translate.z = 10.0f;
	UFOWorld_.Update();

	//天球
	skyDome_ = std::make_unique<SkyDome>();
	skyDome_->Init();
}

void TitleState::Update()
{
	//if (Input::GetPadTrreger()) {
	//	ImGui::Begin("Title");
	//	ImGui::Text("Push");
	//	ImGui::End();
	//}
	ImGui::Begin("Title");
	Vector2 Test = Input::GetPadTrreger();
	ImGui::Text("%f",Test.x);
	ImGui::Text("%f",Test.y);
	ImGui::End();

	skyDome_->Update();
}

void TitleState::Draw()
{
	skyDome_->Draw();

	UFOmodel_->RendererDraw(UFOWorld_);
}

