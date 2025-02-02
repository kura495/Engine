#include "PJump.h"
#include "../../Player.h"
void PJump::Init(Player* p)
{
	p;
	p->jumpForce = kJumpForce;
}

void PJump::Update(Player* p)
{
	//移動関数
	p->Move();
	//ジャンプの処理
	p->GetWorld().transform.translate.y += p->jumpForce;
	p->jumpForce -= kJumpSubValue;
}

void PJump::Draw(Player* p)
{
	p->Getmodels()[0]->RendererDraw(p->GetWorld());
}

std::string PJump::ShowState()
{
	return "Jump";
}
