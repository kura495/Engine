#pragma once
#include "../IBossState.h"
#include "Object/Enemy/Boss/Ball/Ball.h"

class EAttackThrowball : public IBossState
{
public:
	EAttackThrowball() { stateType = BossState::AttackThrowball; };

	void Init(Boss* boss)override;
	void Update(Boss* boss)override;
	void Draw(Boss* boss)override;
	void OnCollisionAttack(Boss* boss, const ICollider& collider)override;
	std::string ShowState()override;

private:
	//AttackThrowBomb
	std::unique_ptr<Ball> ball;
	//SEハンドル
	int SEthrowBall;
	//ボールに当たった回数
	int countHitBall;

	std::unique_ptr<Sprite>ui_;
	WorldTransform ui_world_;
};
