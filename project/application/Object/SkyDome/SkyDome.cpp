#include "SkyDome.h"

void SkyDome::Init()
{
	models_.push_back(Model::CreateModelFromObj("project/resources/SkyDome", "SkyDome.obj"));
	world_.Init();
}

void SkyDome::Update()
{
#ifdef _DEBUG
	ImGui::Begin("SkyDome");
	ImGui::DragFloat3("uvTransform",&uvT.x,0.1f);
	ImGui::DragFloat3("color",&color.x);
	ImGui::End();
	world_.Update();
#endif
	uvT.x += 0.0001f;
	models_[0]->GetMaterial()->uvTransform = MakeTranslateMatrix(uvT);
}

void SkyDome::Draw()
{
	for (Model* model : models_) {
		model->RendererDraw(world_);
	}
}

void SkyDome::ImGui()
{
#ifdef _DEBUG

#endif
}
