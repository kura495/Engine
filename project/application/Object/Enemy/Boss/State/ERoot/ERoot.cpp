#include "ERoot.h"
#include "../../Boss.h"
#include "application/Object/Player/Player.h"
void ERoot::Init(Boss* boss)
{
	easeT = 0.0f;
	//当たり判定を有効か
	boss->SetColliderUse(Boss::ColliderType::WeekPoint,true);
	boss->SetColliderUse(Boss::ColliderType::Arm, true);
	boss->SetColliderUse(Boss::ColliderType::Hund, true);
	//当たり判定を通常に変更
	boss->SetColliderAttribute(Boss::ColliderType::Arm, Collider::Tag::Enemy);
	boss->SetColliderAttribute(Boss::ColliderType::Hund, Collider::Tag::Enemy);
}

void ERoot::Update(Boss* boss)
{
	if (boss->GetPlayer()->GetState() == PlayerState::kDead) {
		return;
	}
	//攻撃をする
	if (boss->isAttackSelect == Boss::AttackState::Throw) {
		//ボールを投げる攻撃
		boss->isAttackSelect = Boss::AttackState::Slam;
		boss->ChangeState<EAttackThrowball>();
	}
	else if (boss->isAttackSelect == Boss::AttackState::Slam) {
		if (boss->FollowPlayer()) {
			//叩きつけ攻撃
			boss->isAttackSelect = Boss::AttackState::RocketPunch;
			boss->ChangeState<EAttackSlam>();
		}
	}
	else if (boss->isAttackSelect == Boss::AttackState::RocketPunch) {
		boss->isAttackSelect = Boss::AttackState::Throw;
		boss->ChangeState<EAttackRocketPunch>();
	}
}

void ERoot::Draw(Boss* boss)
{
	boss->Getmodels()[0]->RendererSkinDraw(boss->GetWorld(), boss->GetAnime()->GetSkinCluster());
}
std::string ERoot::ShowState()
{
	return "ERoot";
}