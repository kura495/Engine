#include "EReturnPosition.h"
#include "../../Boss.h"
void EReturnPosition::Init(Boss* Boss)
{
	easeT = 0.0f;
	addEaseT = 0.02f;
	PrePos = Boss->GetWorld().transform.translate;
}

void EReturnPosition::Update(Boss* Boss)
{
	easeT = (std::min)(easeT + addEaseT, 1.0f);
	Boss->GetWorld().transform.translate = Vector3::Lerp(PrePos, initialPosition, easeT);

	if (easeT == 1.0f) {
		Boss->ChangeState<ERoot>();
	}
	if (easeT >= 0.2f) {
		Boss->isSlamFlag = false;
	}
}

void EReturnPosition::Draw(Boss* Boss)
{
	Boss->Getmodels()[0]->RendererSkinDraw(Boss->GetWorld(), Boss->GetAnime()->GetSkinCluster());
}
std::string EReturnPosition::ShowState()
{
	return "EReturnPosition";
}