#include "EReturnPosition.h"
#include "../../Boss.h"
void EReturnPosition::Init(Boss* Boss)
{
	Boss->ReturnPositionInit();
}

void EReturnPosition::Update(Boss* Boss)
{
	Boss->ReturnPositionUpdate();
}

void EReturnPosition::Draw(Boss* Boss)
{
	Boss->ReturnPositionDraw();
}
std::string EReturnPosition::ShowState()
{
	return "EReturnPosition";
}