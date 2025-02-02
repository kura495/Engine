#pragma once
#include "../IBossState.h"
class EAttackRocketPunch : public IBossState
{
public:
	EAttackRocketPunch() { stateType = BossState::RocketPunth; };

	void Init(Boss* boss)override;
	void Update(Boss* boss)override;
	void Draw(Boss* boss)override;
	std::string ShowState()override;
private:
	//ターゲットに向かうベクトル
	Math::Vector3 forTargetVector;
	Math::Vector3 PrePos;
	float ramdomvalue = 0.1f;
};
