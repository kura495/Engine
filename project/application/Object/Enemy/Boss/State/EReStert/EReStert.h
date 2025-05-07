#pragma once
#include "../IBossState.h"
class EReStert : public IBossState
{
public:

	EReStert() { stateType = BossState::ReStert; };
	void Init(Boss* boss)override;
	void Update(Boss* boss)override;
	void Draw(Boss* boss)override;
	std::string ShowState()override;
private:
	//現在のTの値
	float easeT = 0.0f;
	//ステートを切り替えるまでの時間
	const float kStateChangeSecond = 1.0f;
	//初期位置
	Math::Vector3 initialPosition{ 0.0f,5.5f,35.0f };
};

