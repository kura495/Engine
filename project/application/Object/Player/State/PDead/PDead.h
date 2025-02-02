#pragma once
#include "../IPlayerState.h"
class PDead : public IPlayerState
{
public:
	PDead() { stateType = PlayerState::kDead; };

	void Init(Player* p)override;
	void Update(Player* p)override;
	void Draw(Player* p)override;
	std::string ShowState()override;
private:
	//死んだときにモデルを描画するか
	bool isDeadModelDraw = true;
};

