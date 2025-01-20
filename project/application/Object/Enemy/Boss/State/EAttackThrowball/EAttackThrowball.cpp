#include "EAttackThrowball.h"
#include "../../Boss.h"
void EAttackThrowball::Init(Boss* Boss)
{
	Boss->AttackThrowBallInit();
}

void EAttackThrowball::Update(Boss* Boss)
{
	Boss->AttackThrowBallUpdate();
}

void EAttackThrowball::Draw(Boss* Boss)
{
	Boss->AttackThrowBallDraw();
}
std::string EAttackThrowball::ShowState()
{
	return "EAttackThrowball";
}