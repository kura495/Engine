#include "ESwordAttack.h"
#include "../../Boss.h"
void ESwordAttack::Init(Boss* boss)
{
	beamSword_ = std::make_unique<BeamSword>();
	beamSword_->Init();
	beamSword_->SetParent(&boss->GetWorld());
	phase_ = Phase::Move;

	prePos = boss->GetWorld().transform.translate;
	
	beamSword_->SetModelSize({ 0.0f,0.0f,0.0f });

	easeT = 0.0f;

	addEaseT = 0.02f;
}
void ESwordAttack::Update(Boss* boss)
{
	beamSword_->Update();

	if (phase_ == Phase::Move) {
		boss->GetWorld().transform.translate = Vector3::Lerp(prePos, startPos, easeT);
		easeT = (std::min)(easeT + addEaseT, 1.0f);
		if (easeT == 1.0f) {
			phase_ = Phase::MakeSword;
			easeT = 0.0f;
			addEaseT = 0.015f;
			boss->getCamera()->LockAt(boss->GetWorld());
			boss->getCamera()->SetFOV(60.0f);
		}
	}
	if (phase_ == Phase::MakeSword) {

		beamSword_->SetModelSize({1.0f,1.0f,easeT});
		easeT = (std::min)(easeT + addEaseT, 1.0f);
		if (easeT == 1.0f) {
			phase_ = Phase::Attack;
			easeT = 0.0f;
			addEaseT = 0.002f;
		}
	}
	if (phase_ == Phase::Attack) {
		boss->GetWorld().transform.translate = Vector3::Lerp(startPos, endPos, easeT);
		easeT = (std::min)(easeT + addEaseT, 1.0f);
		addEaseT += 0.0005f;
		if (easeT == 1.0f) {
			phase_ = Phase::standby;
			easeT = 0.0f;
			addEaseT = 0.05f;
		}
	}
	if (phase_ == Phase::standby) {

		easeT = (std::min)(easeT + addEaseT, 1.0f);
		if (easeT == 1.0f) {
			boss->getCamera()->SetFOV(45.0f);
			boss->ChangeState<EReturnPosition>();
		}
	}
}
void ESwordAttack::Draw(Boss* boss)
{
	boss->Getmodels()[Boss::BossModel::MainBody]->RendererDraw(boss->GetWorld());
	beamSword_->Draw();
}
std::string ESwordAttack::ShowState()
{
	return "ESwingSword";
}
