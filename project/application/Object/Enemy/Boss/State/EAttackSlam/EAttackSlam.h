#pragma once
#include "../IBossState.h"
class EAttackSlam : public IBossState
{
public:
	EAttackSlam() { stateType = BossState::AttackSlam; };

	void Init(Boss* boss)override;
	void Update(Boss* boss)override;
	void Draw(Boss* boss)override;
	std::string ShowState()override;
};