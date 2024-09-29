#include "EnemyManager.h"
#include "Game/Object/Player/Player.h"

void EnemyManager::Init(Player* player)
{
	player_ = player;

	enemyModel_.push_back(Model::CreateModelFromObj("resources/Monster", "Monster.gltf"));

	bossSpider_ = std::make_unique<BossSpider>();
	bossSpider_->Init(enemyModel_);
	bossSpider_->SetPlayer(player_);
}

void EnemyManager::Update()
{
	if (bossSpider_->GetIsAlive() == false) {
		isClear = true;
	}

	enemys_.remove_if([this](Enemy* enemy)
		{
			if (enemy->GetIsAlive() == false)
			{
				enemyDeadCount++;
				delete enemy;
				return true;
			}
			return false;
	});

	spawnFlame++;
	if (spawnFlame > KSpawnFlame) {
		spawn++;
		//TODO：いったんコメントアウト
		//Spawn();
		spawnFlame = 0;
	}

	bossSpider_->Update();

	for (Enemy* enemy : enemys_) {
		enemy->Update();
	}

#ifdef _DEBUG
	ImGui();
#endif
}

void EnemyManager::Draw()
{
	bossSpider_->Draw();

	for (Enemy* enemy : enemys_) {
		enemy->Draw();
	}
}

void EnemyManager::Spawn()
{
	Enemy* enemy = new Spider;
	enemy->Init(enemyModel_);
	enemy->SetPlayer(player_);
	enemys_.push_back(enemy);
}

void EnemyManager::ImGui()
{
	ImGui::Begin("EnemyManager");
	if (ImGui::Button("Spawn")) {
		Spawn();
	}
	ImGui::End();
}
