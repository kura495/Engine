#pragma once
#include "../IBossState.h"
class ERoot : public IBossState
{
public:

	ERoot() { stateType = BossState::Root; };
	void Init(Boss* boss)override;
	void Update(Boss* boss)override;
	void Draw(Boss* boss)override;
	std::string ShowState()override;
private:
	bool FollowPlayer(Boss* boss);
	//FollowPlayerの速度の定数
	const float kFollowPlayerSpeed = 0.5f;
	//FollowPlayerがtrueを返す一定距離
	const float kConstantDistance = 0.2f;

};