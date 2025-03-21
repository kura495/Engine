#pragma once
#include "../IPlayerState.h"
class PAttack : public IPlayerState
{
public:
	PAttack();

	void Init(Player* player)override;
	void Update(Player* player)override;
	void Draw(Player* player)override;
	std::string ShowState()override;
private:
	int SEattack;
	int SEHitattack;
	//攻撃の瞬間に向いていた姿勢を表すクォータニオン
	Math::Quaternion attackPosture;
	Animation* attackAnimation;
};

