#pragma once
#include "../IPlayerState.h"
class PRoot : public IPlayerState
{
public:
	PRoot() { stateType = PlayerState::kRoot; };

	void Init(Player* p)override;
	void Update(Player* p)override;
	void Draw(Player* p)override;
	std::string ShowState()override;
};

