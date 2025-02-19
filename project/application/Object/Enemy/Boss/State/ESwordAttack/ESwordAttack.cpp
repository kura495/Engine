#include "ESwordAttack.h"
#include "../../Boss.h"
void ESwordAttack::Init(Boss* boss)
{
	boss;
	beamSword_ = std::make_unique<BeamSword>();
	beamSword_->Init();
	beamSword_->SetParent(&boss->GetWorld());
}
void ESwordAttack::Update(Boss* boss)
{
	ImGui::Begin("Enemy");
	ImGui::InputFloat3("angle", &angle_.x);
	ImGui::End();
	angle_ *= (float)std::numbers::pi / 180.0f;
	boss->GetWorld().transform.quaternion = Quaternion::EulerToQuaterion(angle_);
	beamSword_->Update();
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
