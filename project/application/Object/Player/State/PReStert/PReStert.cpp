#include "PReStert.h"
#include "../../Player.h"
void PReStert::Init(Player* p)
{
	p->GetWorld().transform.translate = { 0.0f,2.0f,-3.0f };
	p->GetWorld().transform.quaternion = Quaternion::IdentityQuaternion();
	p->GetWorld().Update();
}

void PReStert::Update(Player* p)
{
	easeT = (std::min)(easeT + kDeltaTime, 1.0f);
	if (easeT == 1.0f) {
		p->isReStertFlag = true;
		p->ChangeState<PRoot>();
	}
}

void PReStert::Draw(Player* p)
{
	p->Getmodels()[Player::PlayerModel::MainBody]->RendererDraw(p->GetWorld());
}

std::string PReStert::ShowState()
{
	return "ReStert";
}
