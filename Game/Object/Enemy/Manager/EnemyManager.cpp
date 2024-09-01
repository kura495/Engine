#include "EnemyManager.h"
#include "Game/Object/Player/Player.h"

void EnemyManager::Init(Player* player)
{
	player_ = player;

	enemyModel_.push_back(Model::CreateModelFromObj("resources/Monster", "Monster.gltf"));
}

void EnemyManager::Update()
{
	if (enemyDeadCount > kEnemyDeadCount) {
		isClear = true;
	}

	enemys_.remove_if([this](Enemy* enemy)
		{
			if (!enemy->GetIsAlive())
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
		Spawn();
		spawnFlame = 0;
	}

	for (Enemy* enemy : enemys_) {
		enemy->Update();
	}

#ifdef _DEBUG
	ImGui();
#endif
}

void EnemyManager::Draw()
{
	for (Enemy* enemy : enemys_) {
		enemy->Draw();
	}
}

void EnemyManager::Spawn()
{
	Enemy* enemy = new Enemy;
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
