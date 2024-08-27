#pragma once
#include "Game/Object/Enemy/Enemy.h"

class Player;

class EnemyManager {
public:
	void Init(Player* player);
	void Update();
	void Draw();

	std::list<Enemy*> GetList() { return enemys_; };

private:// 関数
	void Spawn();
	void ImGui();
private:// 変数
	std::vector<Model*> enemyModel_;
	std::list<Enemy*> enemys_;
	
	Player* player_;

	const float KSpawnFlame = 360;
	float spawnFlame = 0;
	const int SpawnMax = 3;
	int spawn = 0;
};