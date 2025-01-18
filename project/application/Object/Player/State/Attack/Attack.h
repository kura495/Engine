#pragma once
#include "../IPlayerState.h"
class Attack : public IPlayerState
{
public:
	Attack() { stateType = PlayerState::kAttack; };

	void Init(Player* p)override;
	void Update(Player* p)override;
	void Draw(Player* p)override;
	std::string ShowState()override;
};

