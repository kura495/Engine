#include "PDead.h"
#include "../../Player.h"
void PDead::Init(Player* p)
{
	p->DeadInit();
}

void PDead::Update(Player* p)
{
	p->DeadUpdate();
}

void PDead::Draw(Player* p)
{
	p->DeadDraw();
}

std::string PDead::ShowState()
{
	return "PDead";
}
