#pragma once
//敵キャラクターを管理するクラス//
#include "Object/Enemy/Boss/Boss.h"

class Player;

class EnemyManager {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="player">プレイヤーのポインタ</param>
	void Init(Player* playerlayer);
	void Update();
	void Draw();
	void ReStert();
	//敵キャラクターのリスト
	std::list<Enemy*> GetList() { return enemys_; };
	//定められた条件(ClearJudge)をクリアしていたならtrue
	bool GetisClear() { return isClear; };

	bool GetSlamFlag() { return bossEnemy_->GetSlamFlag(); };

	WorldTransform& GetBossWorld() { return bossEnemy_->GetWorld(); };

	void SetFollowCamera(FollowCamera* followCamera) { bossEnemy_->SetCamera(followCamera); };
private:
	void ImGui();
	//ゲームクリアの条件を記載
	void ClearJudge();

	std::vector<Model*> enemyModel_;
	std::list<Enemy*> enemys_;

	std::unique_ptr<Boss>bossEnemy_;
	
	Player* player_;

	bool isClear = false;
};