#pragma once
#include "../IBossState.h"
class EDead : public IBossState
{
public:
	EDead() { stateType = BossState::Dead; };

	void Init(Boss* boss)override;
	void Update(Boss* boss)override;
	void Draw(Boss* boss)override;
	std::string ShowState()override;
};
