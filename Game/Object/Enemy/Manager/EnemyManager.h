#pragma once
#include "Game/Object/Enemy/Enemy.h"

class Player;

class EnemyManager {
public:
	void Init();
	void Update();
	void Draw();

private:// 関数
	void Spawn();
private:// 変数
	std::vector<Model*> enemyModel_;
	std::list<Enemy*> enemys_;

};