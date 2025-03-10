#include "PReStert.h"
#include "../../Player.h"
void PReStert::Init(Player* player)
{

	player->GetWorld().transform.translate = initialPosition;
	//大きさを元に戻す
	player->GetWorld().transform.scale = { 1.0f,1.0f,1.0f };
	player->GetWorld().transform.quaternion = Quaternion::IdentityQuaternion();
	player->GetWorld().Update();
}
void PReStert::Update(Player* player)
{
	easeT = (std::min)(easeT + kDeltaTime, 1.0f);
	if (easeT == 1.0f) {
		player->isCompleteReStert = true;
		player->ChangeState<PRoot>();
	}
}
void PReStert::Draw(Player* player)
{
	player->Getmodels()[Player::PlayerModel::MainBody]->RendererDraw(player->GetWorld());
}

std::string PReStert::ShowState()
{
	return "ReStert";
}
