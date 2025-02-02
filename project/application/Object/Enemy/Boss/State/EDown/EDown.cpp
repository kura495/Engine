#include "EDown.h"
#include "../../Boss.h"
void EDown::Init(Boss* boss)
{
	boss->SetColliderUse(Boss::ColliderType::Arm, true);
	boss->SetColliderUse(Boss::ColliderType::Hund, true);
	//当たり判定を通常に変更
	boss->SetColliderAttribute(Boss::ColliderType::Arm, Collider::Tag::Enemy);
	boss->SetColliderAttribute(Boss::ColliderType::Hund, Collider::Tag::Enemy);

	boss->isDownStert = true;
	easeT = 0.0f;
	addEaseT = 0.02f;
	PrePos = boss->GetWorld().transform.translate;
	boss->hitCount = 0;
}

void EDown::Update(Boss* boss)
{
	if (boss->isDownStert) {
		boss->GetAnime()->PlayAnimation();
		animationTime_ += kDeltaTime;
		if (animationTime_ > boss->GetAnime()->duration) {
			boss->isDownStert = false;
			animationTime_ = kDeltaTime;
		}
	}

	easeT = (std::min)(easeT + addEaseT, 1.0f);
	boss->GetWorld().transform.translate = Vector3::Lerp(PrePos, boss->DownPosition, easeT);
	//3回攻撃を受けると元の位置に戻す
	if (boss->hitCount == 3) {
		boss->ChangeState<EReturnPosition>();
		boss->SetColliderUse(Boss::ColliderType::WeekPoint, false);
	}
}

void EDown::Draw(Boss* boss)
{
	boss->Getmodels()[0]->RendererSkinDraw(boss->GetWorld(), boss->GetAnime()->GetSkinCluster());
}
std::string EDown::ShowState()
{
	return "EDown";
}