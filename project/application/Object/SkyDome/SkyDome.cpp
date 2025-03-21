#include "SkyDome.h"

void SkyDome::Init()
{
	models_.push_back(Model::CreateModelFromObj("project/resources/SkyDome", "SkyDome.obj"));
	world_.Init();
}

void SkyDome::Update()
{

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
