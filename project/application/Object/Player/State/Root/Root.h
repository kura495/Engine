#pragma once
#include "../IPlayerState.h"
class Root : public IPlayerState
{
public:
	Root() { stateType = PlayerState::kRoot; };

	void Init(Player* p)override;
	void Update(Player* p)override;
	void Draw(Player* p)override;
	std::string ShowState()override;
};

