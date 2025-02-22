#include "ESwordAttack.h"
#include "../../Boss.h"
void ESwordAttack::Init(Boss* boss)
{
	beamSword_ = std::make_unique<BeamSword>();
	beamSword_->Init();
	beamSword_->SetParent(&boss->GetWorld());
	phase_ = Phase::Move;

	prePos = boss->GetWorld().transform.translate;
	preQua = boss->GetWorld().transform.quaternion;

	angle_ = {0.0f,45.0f,0.0f};
	angle_ *= (float)std::numbers::pi / 180.0f;
	startQua = Quaternion::EulerToQuaterion(angle_);
	angle_ = { 0.0f,315.0f,0.0f };
	angle_ *= (float)std::numbers::pi / 180.0f;
	endQua = Quaternion::EulerToQuaterion(angle_);
	
	beamSword_->SetModelSize({ 0.0f,0.0f,0.0f });

	easeT = 0.0f;

	addEaseT = 0.02f;
}
void ESwordAttack::Update(Boss* boss)
{
	beamSword_->Update();

	if (phase_ == Phase::Move) {
		boss->GetWorld().transform.translate = Vector3::Lerp(prePos, startPos, easeT);
		boss->GetWorld().transform.quaternion = Quaternion::Slerp(preQua, startQua, easeT);
		easeT = (std::min)(easeT + addEaseT, 1.0f);
		if (easeT == 1.0f) {
			phase_ = Phase::MakeSword;
			easeT = 0.0f;
			addEaseT = 0.015f;
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
		boss->GetWorld().transform.quaternion = Quaternion::Slerp(startQua, endQua, easeT);
		easeT = (std::min)(easeT + addEaseT, 1.0f);
		addEaseT += 0.001f;
		if (easeT == 1.0f) {
			boss->ChangeState<EReturnPosition>();
		}
	}
}
void ESwordAttack::Draw(Boss* boss)
{
	boss->Getmodels()[Boss::BossModel::MainBody]->RendererSkinDraw(boss->GetWorld(), boss->GetAnime()->GetSkinCluster());
	beamSword_->Draw();
}
std::string ESwordAttack::ShowState()
{
	return "ESwingSword";
}
