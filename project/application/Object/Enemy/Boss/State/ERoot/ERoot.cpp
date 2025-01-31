#include "ERoot.h"
#include "../../Boss.h"
#include "application/Object/Player/Player.h"
void ERoot::Init(Boss* Boss)
{
	//easeT = 0.0f;
	//当たり判定を有効か
	Boss->SetColliderUse(ColliderNumber::WeekPoint,true);
	Boss->SetColliderUse(ColliderNumber::Arm, true);
	Boss->SetColliderUse(ColliderNumber::Hund, true);
	//当たり判定を通常に変更
	Boss->SetColliderAttribute(ColliderNumber::Arm, ColliderTag::Enemy);
	Boss->SetColliderAttribute(ColliderNumber::Hund, ColliderTag::Enemy);

	//colliderAttack.SetcollitionAttribute(ColliderTag::Enemy);
	//colliderAttackA.SetcollitionAttribute(ColliderTag::Enemy);
}

void ERoot::Update(Boss* Boss)
{
	//Boss->RootUpdate();
	if (Boss->GetPlayer()->GetState() == PlayerState::kDead) {
		return;
	}
	//攻撃をする
	if (Boss->isAttackSelect) {
		//ボールを投げる攻撃
		Boss->ChangeState<EAttackThrowball>();
		Boss->isAttackSelect = false;
	}
	else if (Boss->FollowPlayer()) {
		//叩きつけ攻撃
		Boss->ChangeState<EAttackSlam>();
		Boss->isAttackSelect = true;
	}
}

void ERoot::Draw(Boss* Boss)
{
	Boss->Getmodels()[0]->RendererDraw(Boss->GetWorld());
}
std::string ERoot::ShowState()
{
	return "ERoot";
}