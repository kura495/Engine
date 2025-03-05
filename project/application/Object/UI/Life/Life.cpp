#include "Life.h"

void Life::Init(int HP)
{
	textureManager_ = TextureManager::GetInstance();

	sprite_Life = std::make_unique<Sprite>();
	sprite_Life->Initialize({ 0.0f,0.0f }, { 0.0f,WinApp::kClientHeight }, { WinApp::kClientWidth,0.0f }, { WinApp::kClientWidth,WinApp::kClientHeight });
	sprite_Life->TextureHandle = textureManager_->LoadTexture("project/resources/Clear.png");
	sprite_Life_World.Init();
	HP;
	sprite_Number = std::make_unique<Sprite>();
	sprite_Number->Initialize({ 0.0f,0.0f }, { 0.0f,WinApp::kClientHeight }, { WinApp::kClientWidth,0.0f }, { WinApp::kClientWidth,WinApp::kClientHeight });
	sprite_Number->TextureHandle = textureManager_->LoadTexture("project/resources/Clear.png");
	sprite_Number_World.Init();
}

void Life::Animation()
{

}

void Life::Draw()
{
	sprite_Life->Draw(sprite_Life_World);
	sprite_Number->Draw(sprite_Number_World);
}
