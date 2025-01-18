#include "Jump.h"
#include "../../Player.h"
void Jump::Init(Player* p)
{
	p->JumpInit();
}

void Jump::Update(Player* p)
{
	p->JumpUpdate();
}

void Jump::Draw(Player* p)
{
	p->JumpDraw();
}

std::string Jump::ShowState()
{
	return "Jump";
}
