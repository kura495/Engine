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
	//現在のTの値
	float easeT = 0.0f;
};