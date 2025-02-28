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

};

