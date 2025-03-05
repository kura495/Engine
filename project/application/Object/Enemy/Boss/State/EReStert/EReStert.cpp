#include "EReStert.h"
#include "../../Boss.h"
#include "application/Object/Player/Player.h"
void EReStert::Init(Boss* boss)
{
	boss->GetWorld().transform.translate = initialPosition;
	boss->GetWorld().transform.quaternion = Quaternion::IdentityQuaternion();
	easeT = 0.0f;
}

void EReStert::Update(Boss* boss)
{
	easeT = (std::min)(easeT + kDeltaTime, kStateChangeSecond);
	if (easeT == kStateChangeSecond) {
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
