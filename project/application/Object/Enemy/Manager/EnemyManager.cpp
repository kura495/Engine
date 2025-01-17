#include "EnemyManager.h"
#include "Game/Object/Player/Player.h"

void EnemyManager::Init(Player* player)
{
	player_ = player;
	//モデル
	enemyModel_.push_back(Model::CreateModelFromObj("resources/Enemy", "Arm.gltf"));
	enemyModel_.push_back(Model::CreateModelFromObj("resources/Enemy", "Arm.gltf"));
	//ボスの初期化
	bossEnemy_ = std::make_unique<Boss>();
	bossEnemy_->Init(enemyModel_);
	bossEnemy_->SetPlayer(player_);

}

void EnemyManager::Update()
{
	ClearJudge();
	//敵キャラが条件を満たしていたらデリート
	enemys_.remove_if([this](Enemy* enemy)
		{
			if (enemy->GetIsAlive() == false)
			{
				delete enemy;
				return true;
			}
			return false;
	});

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

void EnemyManager::ImGui()
{
	bossEnemy_->ImGui();
	for (Enemy* enemy : enemys_){
		enemy->ImGui();
	}
}

void EnemyManager::ClearJudge()
{
	if (bossEnemy_->GetIsAlive() == false) {
		isClear = true;
	}
}
