#include "OBBColliderModel.h"

void OBBColliderModel::Init()
{
	world_.Initialize();
	model = Model::CreateModelFromObj("resources/ColliderModel/Box", "Box.obj");
}

void OBBColliderModel::Update()
{
	world_.Update();
}

void OBBColliderModel::Draw()
{
	model->WireFrameDraw(world_);
}
