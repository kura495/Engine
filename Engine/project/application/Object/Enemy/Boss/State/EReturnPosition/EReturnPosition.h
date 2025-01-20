#pragma once
#include "../IBossState.h"
class EReturnPosition : public IBossState
{
public:
	EReturnPosition() { stateType = BossState::ReturnPosition; };

	void Init(Boss* boss)override;
	void Update(Boss* boss)override;
	void Draw(Boss* boss)override;
	std::string ShowState()override;
};