#pragma once
#include "../IBossState.h"
#include "Boss/BeamSword/BeamSword.h"
class ESwordAttack : public IBossState
{
public:
	ESwordAttack() { stateType = BossState::BeamSword; };

	void Init(Boss* boss)override;
	void Update(Boss* boss)override;
	void Draw(Boss* boss)override;
	std::string ShowState()override;
private:
	std::unique_ptr<BeamSword> beamSword_;
};