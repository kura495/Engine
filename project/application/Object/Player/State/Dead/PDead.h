#pragma once
#include "../IPlayerState.h"
class PDead : public IPlayerState
{
	void Init(Player* p)override;
	void Update(Player* p)override;
	std::string ShowState()override;
};

