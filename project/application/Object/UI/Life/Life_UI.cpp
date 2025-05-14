#include "Life_UI.h"

void Life_UI::Init(int HP)
{
	//配列に合わせるために-1する
	saveHP = HP;

	textureManager_ = TextureManager::GetInstance();

	for (int i = 0; i <= saveHP; i++) {
		fade[i].OutInit("project/resources/Life.png");
	}
	for (int i = 0; i < 3; i++) {
		sprite_world_[i].Init();
		sprite_world_[i].transform.translate = position[i];
		sprite_world_[i].Update();
	}

	sprite = std::make_unique<Sprite>();
	sprite->Initialize({ 0.0f,0.0f }, { 0.0f,720.0f }, { 1280.0f,0.0f }, { 1280.0f,720.0f });
	sprite->TextureHandle = textureManager_->LoadTexture("project/resources/Empty_Life.png");
}

bool Life_UI::Animation()
{
	if (fade[saveHP].Out(0.5f)) {
		return true;
	}
	return false;
}

void Life_UI::Draw()
{
	for (int i = 0; i < Life_Count::END; i++) {
		sprite->RendererDraw(sprite_world_[i]);
	}
	for (int i = 0; i <= saveHP; i++) {
		fade[i].Draw(sprite_world_[i]);
	}
}
