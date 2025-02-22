#pragma once
#include "../IBossState.h"
#include "Boss/BeamSword/BeamSword.h"
class ESwordAttack : public IBossState
{
public:
	enum Phase {
		Move,//移動
		MakeSword,//剣を作る
		Attack,//攻撃
		standby,//次のステートまで待機
		END,
	};

	ESwordAttack() { stateType = BossState::BeamSword; };

	void Init(Boss* boss)override;
	void Update(Boss* boss)override;
	void Draw(Boss* boss)override;
	std::string ShowState()override;
private:
	std::unique_ptr<BeamSword> beamSword_;

	Math::Vector3 startPos = {-30.0f,0.5f,35.0f};
	Math::Vector3 endPos = {30.0f,0.5f,35.0f};
	Math::Vector3 prePos;

	int phase_ = Phase::Move;
};