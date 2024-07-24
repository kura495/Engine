#include "EnemyManager.h"

void EnemyManager::Init()
{
	enemyModel_.push_back(Model::CreateModelFromObj("resources/human", "human.gltf"));
}

void EnemyManager::Update()
{

}

void EnemyManager::Draw()
{

}

void EnemyManager::Spawn()
{
	Enemy* enemy = new Enemy;
	enemy->Init(enemyModel_);
	enemy->SetPlayer(player_.get());
}
