#include "OBBColliderModel.h"

void OBBColliderModel::Init()
{
	world_.Initialize();
	model = Model::CreateModelFromObj("resources/ColliderModel/Box", "Box.obj");
}

void OBBColliderModel::Update()
{
	world_.UpdateMatrix();
}

void OBBColliderModel::Draw()
{
	model->WireFrameDraw(world_);
}
