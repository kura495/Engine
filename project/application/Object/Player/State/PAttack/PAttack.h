#pragma once
#include "../IPlayerState.h"
class PAttack : public IPlayerState
{
public:
	PAttack() { stateType = PlayerState::kAttack; };

	void Init(Player* p)override;
	void Update(Player* p)override;
	void Draw(Player* p)override;
	std::string ShowState()override;
};

