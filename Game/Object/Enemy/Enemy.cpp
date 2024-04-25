#include "Enemy.h"

void Enemy::Initialize(std::vector<Model*> models)
{
	models_ = models;

	world_.Initialize();

}

void Enemy::Update()
{

}

void Enemy::Draw(const ViewProjection& viewProj)
{
	for (Model* model : models_) {
		model->Draw(world_, viewProj);
	}
}

void Enemy::ImGui()
{

}
