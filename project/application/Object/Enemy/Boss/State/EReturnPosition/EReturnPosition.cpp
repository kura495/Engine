#include "EReturnPosition.h"
#include "../../Boss.h"
void EReturnPosition::Init(Boss* boss)
{
	easeT = 0.0f;
	addEaseT = 0.02f;
	PrePos = boss->GetWorld().transform.translate;
	PreQua = boss->GetWorld().transform.quaternion;
	//当たり判定を通常に変更
	boss->SetColliderAttribute(Boss::ColliderType::Arm, Collider::Tag::Enemy);
	boss->SetColliderAttribute(Boss::ColliderType::Hund, Collider::Tag::Enemy);
}

void EReturnPosition::Update(Boss* boss)
{
	easeT = (std::min)(easeT + addEaseT, 1.0f);
	boss->GetWorld().transform.translate = Vector3::Lerp(PrePos, initialPosition, easeT);
	boss->GetWorld().transform.quaternion = Quaternion::Slerp(PreQua, Quaternion::IdentityQuaternion(), easeT);

	if (easeT == 1.0f) {
		boss->ChangeState<ERoot>();
	}
	if (easeT >= 0.2f) {
		boss->isSlamFlag = false;
	}
}

void EReturnPosition::Draw(Boss* boss)
{
	boss->Getmodels()[Boss::BossModel::MainBody]->RendererSkinDraw(boss->GetWorld(), boss->GetAnime()->GetSkinCluster());
}
std::string EReturnPosition::ShowState()
{
	return "EReturnPosition";
}