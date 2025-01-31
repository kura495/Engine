#pragma once
#include "../IBossState.h"
class EReturnPosition : public IBossState
{
public:
	EReturnPosition() { stateType = BossState::ReturnPosition; };

	void Init(Boss* boss)override;
	void Update(Boss* boss)override;
	void Draw(Boss* boss)override;
	std::string ShowState()override;

private:
	//戻る前の位置を保存する変数
	Math::Vector3 PrePos;
	//固定の位置
	Math::Vector3 initialPosition{ 0.0f,5.5f,35.0f };
};