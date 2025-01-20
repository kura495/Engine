#include "ERoot.h"
#include "../../Boss.h"
void ERoot::Init(Boss* Boss)
{
	Boss->RootInit();
}

void ERoot::Update(Boss* Boss)
{
	Boss->RootUpdate();
}

void ERoot::Draw(Boss* Boss)
{
	Boss->RootDraw();
}
std::string ERoot::ShowState()
{
	return "ERoot";
}