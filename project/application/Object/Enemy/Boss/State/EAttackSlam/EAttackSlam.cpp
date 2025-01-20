#include "EAttackSlam.h"
#include "../../Boss.h"
void EAttackSlam::Init(Boss* Boss)
{
	Boss->AttackSlamInit();
}

void EAttackSlam::Update(Boss* Boss)
{
	Boss->AttackSlamUpdate();
}

void EAttackSlam::Draw(Boss* Boss)
{
	Boss->AttackSlamDraw();
}
std::string EAttackSlam::ShowState()
{
	return "EAttackSlam";
}