#include "PAttack.h"
#include "../../Player.h"
void PAttack::Init(Player* p)
{
	p->AttackInit();
}

void PAttack::Update(Player* p)
{
	p->AttackUpdate();
}

void PAttack::Draw(Player* p)
{
	p->AttackDraw();
}

std::string PAttack::ShowState()
{
	return "Attack";
}
