#include "EDead.h"
#include "../../Boss.h"
void EDead::Init(Boss* Boss)
{
	Boss->DeadInit();
}

void EDead::Update(Boss* Boss)
{
	Boss->DeadUpdate();
}

void EDead::Draw(Boss* Boss)
{
	Boss->DeadDraw();
}
std::string EDead::ShowState()
{
	return "EDead";
}