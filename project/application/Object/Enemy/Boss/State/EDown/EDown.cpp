#include "EDown.h"
#include "../../Boss.h"
void EDown::Init(Boss* Boss)
{
	Boss->DownInit();
}

void EDown::Update(Boss* Boss)
{
	Boss->DownUpdate();
}

void EDown::Draw(Boss* Boss)
{
	Boss->DownDraw();
}
std::string EDown::ShowState()
{
	return "EDown";
}