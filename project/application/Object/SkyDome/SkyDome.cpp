#include "SkyDome.h"

void SkyDome::Init()
{
	models_.push_back(Model::CreateModelFromObj("project/resources/SkyDome", "SkyDome.obj"));
	world_.Initialize();
}

void SkyDome::Update()
{
#ifdef _DEBUG
	ImGui::Begin("SkyDome");
	ImGui::DragFloat3("Scale",&world_.transform.scale.x);
	ImGui::End();
	world_.Update();
#endif 
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
