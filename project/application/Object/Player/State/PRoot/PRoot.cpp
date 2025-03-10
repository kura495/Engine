#include "PRoot.h"
#include "../../Player.h"
void PRoot::Init(Player* player)
{
	player->SetColliderUse(Player::ColliderType::pCollider, true);
	walkanimation->Reset();
	walkanimation->AnimeInit(*player->Getmodels()[Player::PlayerModel::MainBody], true);
}

void PRoot::Update(Player* player)
{
	//動いていたら
	if (player->Move()) {
		//動いていたら歩きモーションを再生
		walkanimation->PlayAnimation();
	}
	//ボタンを押したら攻撃
	if (Input::GetPadPrecede(XINPUT_GAMEPAD_X, 20)) {
		player->ChangeState<PAttack>();
	}
	//ボタンを押したらジャンプ
	else if (Input::IsTriggerPad(XINPUT_GAMEPAD_A)) {
		player->ChangeState<PJump>();
	}
}

void PRoot::Draw(Player* player)
{
	player->Getmodels()[Player::PlayerModel::MainBody]->RendererSkinDraw(player->GetWorld(), walkanimation->GetSkinCluster());
}

std::string PRoot::ShowState()
{
	return "Root";
}
