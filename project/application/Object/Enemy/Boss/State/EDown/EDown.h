#pragma once
#include "../IBossState.h"
class EDown : public IBossState
{
public:
	EDown() { stateType = BossState::Down; };

	void Init(Boss* boss)override;
	void Update(Boss* boss)override;
	void Draw(Boss* boss)override;
	void OnCollision(Boss* boss, const ICollider& collider)override;
	std::string ShowState()override;
private:
	int hitCount = 0;
	const int hitCountMax = 3;
	float animationTime_ = 0.0f;
	//戻る前の位置を保存する変数
	Math::Vector3 PrePos;
	//Down
	Vector3 DownPosition{ 0.0f,0.5f,20.0f };
	bool isDownStert;
};
