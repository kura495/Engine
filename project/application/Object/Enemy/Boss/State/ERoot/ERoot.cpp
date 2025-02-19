#include "ERoot.h"
#include "../../Boss.h"
#include "application/Object/Player/Player.h"

void ERoot::Init(Boss* boss)
{
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
	if (isAttackSelect == AttackState::Throw) {
		//ボールを投げる攻撃
		isAttackSelect = AttackState::Slam;
		boss->ChangeState<EAttackThrowball>();
	}
	else if (isAttackSelect == AttackState::Slam) {
		if (FollowPlayer(boss)) {
			//叩きつけ攻撃
			isAttackSelect = AttackState::RocketPunch;
			boss->ChangeState<EAttackSlam>();
		}
	}
	else if (isAttackSelect == AttackState::RocketPunch) {
		isAttackSelect = AttackState::Throw;
		boss->ChangeState<EAttackRocketPunch>();
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
bool ERoot::FollowPlayer(Boss* boss)
{
	Vector3 temp = boss->GetPlayer()->GetWorld().transform.translate - boss->GetWorld().transform.translate;
	//モデルの中心から手のひらへ
	temp.z += 5.0f;
	temp.y = 0.0f;
	float playerToEnemyLngth = temp.Length();
	temp = temp.Normalize();
	boss->GetWorld().transform.translate += temp * kFollowPlayerSpeed;
	//一定の距離になったら処理を終わる
	if (playerToEnemyLngth <= kConstantDistance) {
		return true;
	}
	return false;
}