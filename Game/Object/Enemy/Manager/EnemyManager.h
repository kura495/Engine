#pragma once
#include "Game/Object/Enemy/Boss/Boss.h"

class Player;

class EnemyManager {
public:
	void Init(Player* player);
	void Update();
	void Draw();

	std::list<Enemy*> GetList() { return enemys_; };

	bool GetisClear() { return isClear; };

private:// 関数
	void ImGui();
private:// 変数
	std::vector<Model*> enemyModel_;
	std::list<Enemy*> enemys_;

	std::unique_ptr<Boss>bossEnemy_;
	//std::unique_ptr<BossSpider>bossEnemy_;
	
	Player* player_;

	bool isClear = false;
};