#include "EnemyManager.h"
#include "Game/Object/Player/Player.h"

void EnemyManager::Init(Player* player)
{
	player_ = player;

	enemyModel_.push_back(Model::CreateModelFromObj("resources/human", "human.gltf"));
	//TODO:敵を生成
	Spawn();
}

void EnemyManager::Update()
{
	for (Enemy* enemy : enemys_) {
		enemy->Update();
	}
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

}
