#include "BoxColliderModel.h"

void BoxColliderModel::Init()
{
	world_.Init();
	model = Model::CreateModelFromObj("project/resources/ColliderModel/Box", "Box.obj");
}

void BoxColliderModel::Update()
{
	world_.Update();
}

void BoxColliderModel::Draw()
{
	model->WireFrameDraw(world_);
}
