#include "IObject.h"

void IObject::Init(std::vector<Model*> models)
{
	models_ = models;
	world_.Initialize();
}

void IObject::Update()
{
	world_.UpdateMatrix();
}

void IObject::Draw()
{
	for (Model* model : models_) {
		model->RendererDraw(world_);
	}
}
