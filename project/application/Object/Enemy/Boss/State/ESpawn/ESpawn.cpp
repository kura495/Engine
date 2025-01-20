#include "ESpawn.h"
#include "../../Boss.h"
void ESpawn::Init(Boss* Boss)
{
	Boss->SpawnInit();
}

void ESpawn::Update(Boss* Boss)
{
	Boss->SpawnUpdate();
}

void ESpawn::Draw(Boss* Boss)
{
	Boss->SpawnDraw();
}
std::string ESpawn::ShowState()
{
	return "ESpawn";
}