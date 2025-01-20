#pragma once
#include <string>
//前方宣言
class Boss;
enum class BossState {
	Root,
	ReturnPosition,//元の位置に戻る
	AttackSlam,//叩きつけ攻撃
	AttackThrowball,//爆弾を投げる攻撃
	Spawn,//出現時
	Dead,//死亡時
	Down,//ダウン時
};
class IBossState
{
public:
	IBossState() {};
	virtual ~IBossState() {};

	virtual void Init(Boss* p) = 0;
	virtual void Update(Boss* p) = 0;
	virtual void Draw(Boss* p) = 0;
	BossState GetStateType() { return stateType; };
	virtual std::string ShowState() = 0;
	BossState stateType;
protected:
};