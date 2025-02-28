#include "EReStert.h"
#include "../../Boss.h"
#include "application/Object/Player/Player.h"
void EReStert::Init(Boss* boss)
{
}

void EReStert::Update(Boss* boss)
{
	boss->ChangeState<ERoot>();
}

void EReStert::Draw(Boss* boss)
{
	boss->Draw();
}

std::string EReStert::ShowState()
{
	return "EReStert";
}
