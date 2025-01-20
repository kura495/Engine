#pragma once
#include "../IBossState.h"
class EAttackThrowball : public IBossState
{
public:
	EAttackThrowball() { stateType = BossState::AttackThrowball; };

	void Init(Boss* boss)override;
	void Update(Boss* boss)override;
	void Draw(Boss* boss)override;
	std::string ShowState()override;
};
