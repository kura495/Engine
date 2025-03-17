#include "Life_UI.h"

void Life_UI::Init(int HP)
{
	HP;

	textureManager_ = TextureManager::GetInstance();

	fade[0].OutInit("project/resources/Life.png");
	fade[1].OutInit("project/resources/Life.png");
	fade[2].OutInit("project/resources/Life.png");

	sprite_world_[0].Init();
	sprite_world_[0].transform.translate = {-100.0f,0.0f,0.0f};
	sprite_world_[0].Update();

	sprite_world_[1].Init();
	sprite_world_[1].transform.translate = { 0.0f,0.0f,0.0f };
	sprite_world_[1].Update();

	sprite_world_[2].Init();
	sprite_world_[2].transform.translate = { 100.0f,0.0f,0.0f };
	sprite_world_[2].Update();

	/*sprite_world_.Init();
	sprite = std::make_unique<Sprite>();
	sprite->TextureHandle = textureManager_->LoadTexture("project/resources/Life.png");
	sprite->Initialize({ 0.0f,0.0f }, { 0.0f,720.0f }, { 1280.0f,0.0f }, { 1280.0f,720.0f });*/
}

bool Life_UI::Animation()
{
	fade[0].Out(3.0f);
	fade[1].Out(3.0f);
	if (fade[2].Out(3.0f)) {
		return true;
	}
	return false;
}

void Life_UI::Draw()
{
	fade[0].Draw(sprite_world_[0]);
	fade[1].Draw(sprite_world_[1]);
	fade[2].Draw(sprite_world_[2]);
	//sprite->RendererDraw(sprite_world_);
}
