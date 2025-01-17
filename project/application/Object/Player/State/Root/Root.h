#pragma once
#include "../IPlayerState.h"
class Root : public IPlayerState
{
	void Init(Player* p)override;
	void Update(Player* p)override;
};

