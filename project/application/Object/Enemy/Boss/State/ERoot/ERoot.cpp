#include "ERoot.h"
#include "../../Boss.h"
#include "application/Object/Player/Player.h"
void ERoot::Init(Boss* Boss)
{
	easeT = 0.0f;
	//当たり判定を有効か
	Boss->SetColliderUse(Boss::ColliderType::WeekPoint,true);
	Boss->SetColliderUse(Boss::ColliderType::Arm, true);
	Boss->SetColliderUse(Boss::ColliderType::Hund, true);
	//当たり判定を通常に変更
	Boss->SetColliderAttribute(Boss::ColliderType::Arm, Collider::Tag::Enemy);
	Boss->SetColliderAttribute(Boss::ColliderType::Hund, Collider::Tag::Enemy);
}

void ERoot::Update(Boss* Boss)
{
	if (Boss->GetPlayer()->GetState() == PlayerState::kDead) {
		return;
	}
	//攻撃をする
	if (Boss->isAttackSelect) {
		//ボールを投げる攻撃
		Boss->isAttackSelect = false;
		Boss->ChangeState<EAttackThrowball>();
	}
	else if (Boss->FollowPlayer()) {
		//叩きつけ攻撃
		Boss->isAttackSelect = true;
		Boss->ChangeState<EAttackSlam>();
	}
}

void ERoot::Draw(Boss* Boss)
{
	Boss->Getmodels()[0]->RendererSkinDraw(Boss->GetWorld(),Boss->GetAnime()->GetSkinCluster());
}
std::string ERoot::ShowState()
{
	return "ERoot";
}