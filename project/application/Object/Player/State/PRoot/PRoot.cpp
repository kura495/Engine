#include "PRoot.h"
#include "../../Player.h"
void PRoot::Init(Player* p)
{
	p->SetColliderUse(Player::ColliderType::pCollider, true);
	walkanimation->Reset();
	walkanimation->AnimeInit(*p->Getmodels()[Player::PlayerModel::MainBody], true);
}

void PRoot::Update(Player* p)
{
	//動いていたら
	if (p->Move()) {
		//動いていたら歩きモーションを再生
		walkanimation->PlayAnimation();
	}
	//ボタンを押したら攻撃
	if (Input::GetPadPrecede(XINPUT_GAMEPAD_X, 20)) {
		p->ChangeState<PAttack>();
	}
	//ボタンを押したらジャンプ
	else if (Input::IsTriggerPad(XINPUT_GAMEPAD_A)) {
		p->ChangeState<PJump>();
	}
}

void PRoot::Draw(Player* p)
{
	p->Getmodels()[Player::PlayerModel::MainBody]->RendererSkinDraw(p->GetWorld(), walkanimation->GetSkinCluster());
}

std::string PRoot::ShowState()
{
	return "Root";
}
