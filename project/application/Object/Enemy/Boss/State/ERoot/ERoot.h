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

};