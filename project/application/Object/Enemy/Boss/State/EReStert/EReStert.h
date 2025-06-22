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
	//現在のTの値
	float easeT = 0.0f;
	//ステートを切り替えるまでの時間
	const float kMaxEaseValue = 1.5f;
	//戻る前の位置を保存する変数
	Math::Vector3 PrePos = { 0.0f,0.0f,0.0f };
	//戻る前の回転を保存する変数
	Math::Quaternion PreQua = Math::Quaternion::IdentityQuaternion();
	//固定の位置
	Math::Vector3 initialPosition{ 0.0f,5.5f,35.0f };
};

