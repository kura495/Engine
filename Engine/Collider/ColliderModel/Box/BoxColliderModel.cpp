#include "BoxColliderModel.h"

void BoxColliderModel::Init()
{
	world_.Initialize();
	model = Model::CreateModelFromObj("resources/ColliderModel/Box", "Box.obj");
}

void BoxColliderModel::Update()
{
	world_.Update();
}

void BoxColliderModel::Draw()
{
	model->WireFrameDraw(world_);
}
