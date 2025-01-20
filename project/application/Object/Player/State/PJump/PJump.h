#pragma once
#include "../IPlayerState.h"
class PJump : public IPlayerState
{
public:
	PJump() { stateType = PlayerState::kJump; };

	void Init(Player* p)override;
	void Update(Player* p)override;
	void Draw(Player* p)override;
	std::string ShowState()override;
};

