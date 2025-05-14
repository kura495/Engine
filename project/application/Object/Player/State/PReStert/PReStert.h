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
	//ステートを切り替えるまでの時間
	const float kMaxEaseValue = 1.5f;
	//初期位置
	Math::Vector3 initialPosition{ 0.0f,0.0f,-3.0f };


};