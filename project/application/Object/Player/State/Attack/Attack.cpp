#include "Attack.h"
#include "../../Player.h"
void Attack::Init(Player* p)
{
	p->AttackInit();
}

void Attack::Update(Player* p)
{
	p->AttackUpdate();
}

void Attack::Draw(Player* p)
{
	p->AttackDraw();
}

std::string Attack::ShowState()
{
	return "Attack";
}
