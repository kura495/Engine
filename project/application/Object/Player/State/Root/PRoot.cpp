#include "PRoot.h"
#include "../../Player.h"
void PRoot::Init(Player* p)
{
	p->RootInit();
}

void PRoot::Update(Player* p)
{
	p->RootUpdate();
}

void PRoot::Draw(Player* p)
{
	p->RootDraw();
}

std::string PRoot::ShowState()
{
	return "Root";
}
