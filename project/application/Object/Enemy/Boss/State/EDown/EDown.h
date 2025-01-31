#pragma once
#include "../IBossState.h"
class EDown : public IBossState
{
public:
	EDown() { stateType = BossState::Down; };

	void Init(Boss* boss)override;
	void Update(Boss* boss)override;
	void Draw(Boss* boss)override;
	std::string ShowState()override;
private:
	float animationTime_ = 0.0f;
	//戻る前の位置を保存する変数
	Math::Vector3 PrePos;
};
