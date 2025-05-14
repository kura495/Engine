#include "EReStert.h"
#include "../../Boss.h"
#include "application/Object/Player/Player.h"
void EReStert::Init(Boss* boss)
{
	boss->GetWorld().transform.translate = initialPosition;
	boss->GetWorld().transform.quaternion = Quaternion::IdentityQuaternion();
	boss->GetWorld().Update();
	easeT = 0.0f;
	isAttackSelect = AttackState::RocketPunch;
}

void EReStert::Update(Boss* boss)
{
	easeT = (std::min)(easeT + kDeltaTime, kMaxEaseValue);
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
