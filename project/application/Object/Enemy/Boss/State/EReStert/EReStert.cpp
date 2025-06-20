#include "EReStert.h"
#include "../../Boss.h"
#include "application/Object/Player/Player.h"
void EReStert::Init(Boss* boss)
{
	PrePos = boss->GetWorld().transform.translate;
	PreQua = boss->GetWorld().transform.quaternion;
	easeT = 0.0f;
	isAttackSelect = AttackState::RocketPunch;
}

void EReStert::Update(Boss* boss)
{

	easeT = (std::min)(easeT + kDeltaTime, kMaxEaseValue);
	boss->GetWorld().transform.translate = Vector3::Lerp(PrePos, initialPosition, easeT);
	boss->GetWorld().transform.quaternion = Quaternion::Slerp(PreQua, Quaternion::IdentityQuaternion(), easeT);
	if (easeT == kMaxEaseValue) {
		boss->ChangeState<ERoot>();
	}
}

void EReStert::Draw(Boss* boss)
{
	boss->Getmodels()[Boss::BossModel::MainBody]->RendererDraw(boss->GetWorld());
}

std::string EReStert::ShowState()
{
	return "EReStert";
}
