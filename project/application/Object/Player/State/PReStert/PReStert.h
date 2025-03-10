#pragma once
#include "../IPlayerState.h"
class PReStert : public IPlayerState {
public:
	PReStert() { stateType = PlayerState::ReStert; };

	void Init(Player* player)override;
	void Update(Player* player)override;
	void Draw(Player* player)override;
	std::string ShowState()override;
private:
	//現在のTの値
	float easeT = 0.0f;
};