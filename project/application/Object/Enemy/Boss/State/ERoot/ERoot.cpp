#include "ERoot.h"
#include "../../Boss.h"
#include "application/Object/Player/Player.h"

void ERoot::Init(Boss* boss)
{
	//当たり判定を有効か
	boss->SetColliderUse(Boss::ColliderType::DamageArm,true);
	boss->SetColliderUse(Boss::ColliderType::DamageHund,true);

	boss->SetColliderUse(Boss::ColliderType::AttackArm, true);
	boss->SetColliderUse(Boss::ColliderType::AttackHund, true);
	//当たり判定を通常に変更
	boss->SetColliderAttribute(Boss::ColliderType::AttackArm, Collider::Tag::Enemy);
	boss->SetColliderAttribute(Boss::ColliderType::AttackHund, Collider::Tag::Enemy);
}

void ERoot::Update(Boss* boss)
{
	if (boss->GetPlayer()->GetState() == PlayerState::Dead) {
		return;
	}
	if (boss->isDamage) {
		return;
	}
	//攻撃をする
	if (isAttackSelect == AttackState::RocketPunch) {
		boss->ChangeState<EAttackRocketPunch>();
		//次の攻撃を設定
		isAttackSelect = AttackState::Throw;
	}
	else if (isAttackSelect == AttackState::Throw) {
		//ボールを投げる攻撃
		boss->ChangeState<EAttackThrowball>();
		//次の攻撃を設定
		isAttackSelect = AttackState::Slam;
	}
	else if (isAttackSelect == AttackState::Slam) {
		//叩きつけ攻撃
		boss->ChangeState<EAttackSlam>();
		//次の攻撃を設定
		isAttackSelect = AttackState::SwordSlash;
	}
	else if (isAttackSelect == AttackState::SwordSlash) {
		boss->ChangeState<ESwordAttack>();
		//次の攻撃を設定
		isAttackSelect = AttackState::RocketPunch;
	}
}

void ERoot::Draw(Boss* boss)
{
	boss->Getmodels()[Boss::BossModel::MainBody]->RendererSkinDraw(boss->GetWorld(), boss->GetAnime()->GetSkinCluster());
}
std::string ERoot::ShowState()
{
	return "ERoot";
}
