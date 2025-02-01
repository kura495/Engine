#include "PJump.h"
#include "../../Player.h"
void PJump::Init(Player* p)
{
	p->jumpForce = p->kJumpForce;
}

void PJump::Update(Player* p)
{
	//移動関数
	p->Move();
	//ジャンプの処理
	p->GetWorld().transform.translate.y += p->jumpForce;
	p->jumpForce -= p->kJumpSubValue;
}

void PJump::Draw(Player* p)
{
	p->Getmodels()[0]->RendererSkinDraw(p->GetWorld(), p->walkanimation->GetSkinCluster());
}

std::string PJump::ShowState()
{
	return "Jump";
}
