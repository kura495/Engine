#include "EnemyManager.h"
#include "Game/Object/Player/Player.h"

void EnemyManager::Init(Player* player)
{
	player_ = player;

	enemyModel_.push_back(Model::CreateModelFromObj("resources/Monster", "Monster.gltf"));
	enemyModel_.push_back(Model::CreateModelFromObj("resources/Weapon", "Weapon.obj"));

	bossEnemy_ = std::make_unique<Boss>();
	//bossEnemy_ = std::make_unique<BossSpider>();
	bossEnemy_->Init(enemyModel_);
	bossEnemy_->SetPlayer(player_);


}

void EnemyManager::Update()
{
	if (bossEnemy_->GetIsAlive() == false) {
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

	bossEnemy_->Update();

	for (Enemy* enemy : enemys_) {
		enemy->Update();
	}

#ifdef _DEBUG
	ImGui();
#endif
}

void EnemyManager::Draw()
{
	bossEnemy_->Draw();

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
