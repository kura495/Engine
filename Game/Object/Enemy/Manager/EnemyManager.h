#pragma once
#include "Game/Object/Enemy/Spider/Spider.h"
#include "Game/Object/Enemy/BossSpider/BossSpider.h"

class Player;

class EnemyManager {
public:
	void Init(Player* player);
	void Update();
	void Draw();

	std::list<Enemy*> GetList() { return enemys_; };

	bool GetisClear() { return isClear; };

private:// 関数
	void Spawn();
	void ImGui();
private:// 変数
	std::vector<Model*> enemyModel_;
	std::list<Enemy*> enemys_;

	std::unique_ptr<BossSpider>bossSpider_;
	
	Player* player_;

	const float KSpawnFlame = 360;
	float spawnFlame = 0;
	const int SpawnMax = 3;
	int spawn = 0;

	int enemyDeadCount = 0;
	const int kEnemyDeadCount = 4;

	bool isClear = false;
};