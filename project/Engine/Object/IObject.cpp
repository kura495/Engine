#include "IObject.h"

void IObject::Init(std::vector<Model*> models)
{
	models_ = models;
	world_.Init();
}

void IObject::Update()
{
	world_.Update();
}

void IObject::Draw()
{
	for (Model* model : models_) {
		model->RendererDraw(world_);
	}
}
