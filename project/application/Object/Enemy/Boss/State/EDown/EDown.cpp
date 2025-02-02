#include "EDown.h"
#include "../../Boss.h"
void EDown::Init(Boss* Boss)
{
	Boss->SetColliderUse(Boss::ColliderType::Arm, true);
	Boss->SetColliderUse(Boss::ColliderType::Hund, true);
	//当たり判定を通常に変更
	Boss->SetColliderAttribute(Boss::ColliderType::Arm, Collider::Tag::Enemy);
	Boss->SetColliderAttribute(Boss::ColliderType::Hund, Collider::Tag::Enemy);

	Boss->isDownStert = true;
	easeT = 0.0f;
	addEaseT = 0.02f;
	PrePos = Boss->GetWorld().transform.translate;
	Boss->hitCount = 0;
}

void EDown::Update(Boss* Boss)
{
	if (Boss->isDownStert) {
		Boss->GetAnime()->PlayAnimation();
		animationTime_ += 1.0f / 60.0f;
		if (animationTime_ > Boss->GetAnime()->duration) {
			Boss->isDownStert = false;
			animationTime_ = 0.0f;
		}
	}

	easeT = (std::min)(easeT + addEaseT, 1.0f);
	Boss->GetWorld().transform.translate = Vector3::Lerp(PrePos, Boss->DownPosition, easeT);
	//3回攻撃を受けると元の位置に戻す
	if (Boss->hitCount == 3) {
		Boss->ChangeState<EReturnPosition>();
		Boss->SetColliderUse(Boss::ColliderType::WeekPoint, false);
	}
}

void EDown::Draw(Boss* Boss)
{
	Boss->Getmodels()[0]->RendererSkinDraw(Boss->GetWorld(), Boss->GetAnime()->GetSkinCluster());
}
std::string EDown::ShowState()
{
	return "EDown";
}