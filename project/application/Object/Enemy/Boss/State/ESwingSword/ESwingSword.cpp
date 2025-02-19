#include "ESwingSword.h"
#include "../../Boss.h"
void ESwingSword::Init(Boss* boss)
{
	boss;
	sword_ = std::make_unique<Sword>();
	sword_->Init();
}
void ESwingSword::Update(Boss* boss)
{
	boss;
	sword_->Update();
}
void ESwingSword::Draw(Boss* boss)
{
	boss->Getmodels()[Boss::BossModel::MainBody]->RendererSkinDraw(boss->GetWorld(), boss->GetAnime()->GetSkinCluster());
	sword_->Draw();
}
std::string ESwingSword::ShowState()
{
	return "ESwingSword";
}
