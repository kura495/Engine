#pragma once
#include "../IPlayerState.h"
class PJump : public IPlayerState
{
public:
	PJump() { stateType = PlayerState::Jump; };

	void Init(Player* player)override;
	void Update(Player* player)override;
	void Draw(Player* player)override;
	void OnCollision(Player* player,const ICollider& collider)override;
	std::string ShowState()override;
private:
	//ジャンプに使う実数値
	float jumpForce = 0.0f;
	//ジャンプの強さ
	const float kJumpForce = 0.5f;
	//ジャンプした時の減算
	const float kJumpSubValue = 0.03f;
};

