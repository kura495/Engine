#pragma once
#include "../IPlayerState.h"
class PReStert : public IPlayerState {
public:
	PReStert() { stateType = PlayerState::ReStert; };

	void Init(Player* p)override;
	void Update(Player* p)override;
	void Draw(Player* p)override;
	std::string ShowState()override;
private:
};