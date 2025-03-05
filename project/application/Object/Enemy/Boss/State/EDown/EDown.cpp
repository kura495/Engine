#include "EDown.h"
#include "../../Boss.h"
void EDown::Init(Boss* boss)
{
	boss->SetColliderUse(Boss::ColliderType::AttackArm, true);
	boss->SetColliderUse(Boss::ColliderType::AttackHund, true);
	//当たり判定を通常に変更
	boss->SetColliderAttribute(Boss::ColliderType::AttackArm, Collider::Tag::Enemy);
	boss->SetColliderAttribute(Boss::ColliderType::AttackHund, Collider::Tag::Enemy);

	isDownStert = true;
	easeT = 0.0f;
	addEaseT = 0.02f;
	PrePos = boss->GetWorld().transform.translate;
	hitCount = 0;
}

void EDown::Update(Boss* boss)
{
	if (isDownStert) {
		boss->GetAnime()->PlayAnimation();
		animationTime_ += kDeltaTime;
		if (animationTime_ > boss->GetAnime()->duration) {
			isDownStert = false;
			animationTime_ = kDeltaTime;
		}
	}

	easeT = (std::min)(easeT + addEaseT, 1.0f);
	boss->GetWorld().transform.translate = Vector3::Lerp(PrePos, DownPosition, easeT);
	//3回攻撃を受けると元の位置に戻す
	if (hitCount == 3) {
		boss->ChangeState<EReturnPosition>();
		boss->SetColliderUse(Boss::ColliderType::DamageArm, false);
		boss->SetColliderUse(Boss::ColliderType::DamageHund, false);
	}
}

void EDown::Draw(Boss* boss)
{
	boss->Getmodels()[Boss::BossModel::MainBody]->RendererSkinDraw(boss->GetWorld(), boss->GetAnime()->GetSkinCluster());
}
void EDown::OnCollisionDamage(Boss* boss, const ICollider& collider)
{
	boss;
	if (collider.GetcollitionAttribute() == Collider::Tag::Weapon) {
		hitCount += 1;
	}
}
std::string EDown::ShowState()
{
	return "EDown";
}