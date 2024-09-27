#include "BossSpider.h"

void BossSpider::Init(std::vector<Model*> models)
{
	models_ = models;
	world_.Initialize();

	//Collider
	InitCollider();

	world_.transform.translate.y = -1.0f;
}

void BossSpider::Update()
{
}

void BossSpider::Draw()
{
	for (Model* model : models_) {
		model->RendererDraw(world_);
	}
}

void BossSpider::InitCollider()
{

}
