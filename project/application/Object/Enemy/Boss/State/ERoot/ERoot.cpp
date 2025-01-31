#include "ERoot.h"
#include "../../Boss.h"
#include "application/Object/Player/Player.h"
void ERoot::Init(Boss* Boss)
{
	Boss->RootInit();
	//easeT = 0.0f;
	//当たり判定を有効か
	//colliderDamage.IsUsing = true;
	//colliderAttack.IsUsing = true;
	//colliderAttackA.IsUsing = true;
	////当たり判定を通常に変更
	//colliderAttack.SetcollitionAttribute(ColliderTag::Enemy);
	//colliderAttackA.SetcollitionAttribute(ColliderTag::Enemy);
}

void ERoot::Update(Boss* Boss)
{
	Boss->RootUpdate();
	//if (Boss->GetPlayer()->GetState() == PlayerState::kDead) {
	//	return;
	//}
	////攻撃をする
	//if (Boss->isAttackSelect) {
	//	//ボールを投げる攻撃
	//	Boss->ChangeState<EAttackThrowball>();
	//	Boss->isAttackSelect = false;
	//}
	//else if (Boss->FollowPlayer()) {
	//	//叩きつけ攻撃
	//	Boss->ChangeState<EAttackSlam>();
	//	Boss->isAttackSelect = true;
	//}
}

void ERoot::Draw(Boss* Boss)
{
	Boss->RootDraw();
	//Boss->Getmodels()[0]->RendererDraw(Boss->GetWorld());
}
std::string ERoot::ShowState()
{
	return "ERoot";
}