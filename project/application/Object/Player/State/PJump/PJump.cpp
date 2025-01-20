#include "PJump.h"
#include "../../Player.h"
void PJump::Init(Player* p)
{
	p->JumpInit();
}

void PJump::Update(Player* p)
{
	p->JumpUpdate();
}

void PJump::Draw(Player* p)
{
	p->JumpDraw();
}

std::string PJump::ShowState()
{
	return "Jump";
}
