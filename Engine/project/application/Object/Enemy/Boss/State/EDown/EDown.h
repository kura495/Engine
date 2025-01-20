#pragma once
#include "../IBossState.h"
class EDown : public IBossState
{
public:
	EDown() { stateType = BossState::Down; };

	void Init(Boss* boss)override;
	void Update(Boss* boss)override;
	void Draw(Boss* boss)override;
	std::string ShowState()override;
};
