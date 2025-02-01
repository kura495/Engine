#include "PRoot.h"
#include "../../Player.h"
void PRoot::Init(Player* p)
{
	p->colliderPlayer.IsUsing = true;
	p->animationTime_ = 0.0f;
	p->walkanimation->Reset();
	p->isMovedFlag = false;
}

void PRoot::Update(Player* p)
{
	p->Move();
	//動いていたら
	if (p->isMovedFlag) {
		//動いていたら歩きモーションを再生
		p->walkanimation->PlayAnimation();

		p->animationTime_ += 1.0f / 60.0f;
		if (p->animationTime_ > p->deadAnimation->duration) {
			p->animationTime_ = 0.0f;
			p->isMovedFlag = false;
		}
	}
	//重力
	p->GetWorld().transform.translate.y -= p->gravity;
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
	p->Getmodels()[0]->RendererSkinDraw(p->GetWorld(), p->walkanimation->GetSkinCluster());
}

std::string PRoot::ShowState()
{
	return "Root";
}
