#include "Life_UI.h"

void Life_UI::Init(int HP)
{
	HP;
	textureManager_ = TextureManager::GetInstance();

	fade1.OutInit("project/resources/Life.png");
	fade2.OutInit("project/resources/Life.png");
	fade3.OutInit("project/resources/Life.png");

	sprite_world_1.Init();
	sprite_world_1.transform.translate = { -100.0f,0.0f,0.0f };
	sprite_world_1.Update();

	sprite_world_2.Init();
	sprite_world_2.transform.translate = { 0.0f,0.0f,0.0f };
	sprite_world_2.Update();

	sprite_world_3.Init();
	sprite_world_3.transform.translate = { 100.0f,0.0f,0.0f };
	sprite_world_3.Update();

	/*sprite_world_.Init();
	sprite = std::make_unique<Sprite>();
	sprite->TextureHandle = textureManager_->LoadTexture("project/resources/Life.png");
	sprite->Initialize({ 0.0f,0.0f }, { 0.0f,720.0f }, { 1280.0f,0.0f }, { 1280.0f,720.0f });*/
}

bool Life_UI::Animation()
{
	fade1.Out(3.0f);
	fade2.Out(3.0f);
	if (fade3.Out(3.0f)) {
		return true;
	}
	return false;
}

void Life_UI::Draw()
{
	fade1.Draw(sprite_world_1);
	fade2.Draw(sprite_world_2);
	fade3.Draw(sprite_world_3);
	//sprite->RendererDraw(sprite_world_);
}
