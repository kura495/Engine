#pragma once
#include "../IPlayerState.h"
class PJump : public IPlayerState
{
public:
	PJump() { stateType = PlayerState::kJump; };

	void Init(Player* p)override;
	void Update(Player* p)override;
	void Draw(Player* p)override;
	void OnCollision(Player* p,const ICollider& collider)override;
	std::string ShowState()override;
private:
	//ジャンプに使う実数値
	float jumpForce = 0.0f;
	//ジャンプの強さ
	const float kJumpForce = 0.5f;
	//ジャンプした時の減算
	const float kJumpSubValue = 0.03f;
};

