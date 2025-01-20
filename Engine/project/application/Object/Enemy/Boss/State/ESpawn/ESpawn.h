#pragma once
#include "../IBossState.h"
class ESpawn : public IBossState
{
public:
	ESpawn() { stateType = BossState::Spawn; };

	void Init(Boss* boss)override;
	void Update(Boss* boss)override;
	void Draw(Boss* boss)override;
	std::string ShowState()override;
};