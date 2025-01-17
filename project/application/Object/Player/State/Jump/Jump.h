#pragma once
#include "../IPlayerState.h"
class Jump : public IPlayerState
{
	void Init(Player* p)override;
	void Update(Player* p)override;
};

