#include "ESwordAttack.h"
#include "../../Boss.h"
void ESwordAttack::Init(Boss* boss)
{
	boss;
	beamSword_ = std::make_unique<BeamSword>();
	beamSword_->Init();
}
void ESwordAttack::Update(Boss* boss)
{
	boss;
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
