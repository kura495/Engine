#pragma once
#include "../IBossState.h"
class EAttackSlam : public IBossState
{
public:
	EAttackSlam() { stateType = BossState::AttackSlam; };

	void Init(Boss* boss)override;
	void Update(Boss* boss)override;
	void Draw(Boss* boss)override;
	void OnCollisionAttack(Boss* boss,const ICollider& collider)override;
	std::string ShowState()override;
private:
	bool FollowPlayer(Boss* boss);
	//FollowPlayerの速度の定数
	const float kFollowPlayerSpeed = 0.5f;
	//FollowPlayerがtrueを返す一定距離
	const float kConstantDistance = 0.2f;

	//AttackSlamPlayer
	bool IsAttackFlag = false;
	//叩きつけを一回以上しているか
	bool isSlam2ndFlag = true;
	//SEハンドル
	int SEHitattack;
};