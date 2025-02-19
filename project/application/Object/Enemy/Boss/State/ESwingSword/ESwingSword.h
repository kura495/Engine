#pragma once
#include "../IBossState.h"
#include "Boss/Sword/Sword.h"
class ESwingSword : public IBossState
{
public:
	ESwingSword() { stateType = BossState::SwingSword; };

	void Init(Boss* boss)override;
	void Update(Boss* boss)override;
	void Draw(Boss* boss)override;
	std::string ShowState()override;
private:
	std::unique_ptr<Sword> sword_;
};