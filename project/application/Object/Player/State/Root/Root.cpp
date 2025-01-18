#include "Root.h"
#include "../../Player.h"
void Root::Init(Player* p)
{
	p->RootInit();
}

void Root::Update(Player* p)
{
	p->RootUpdate();
}

void Root::Draw(Player* p)
{
	p->RootDraw();
}

std::string Root::ShowState()
{
	return "Root";
}
