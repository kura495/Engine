#include "OBBColliderModel.h"

void OBBColliderModel::Init()
{
	world_.Init();
	model = Model::CreateModelFromObj("project/resources/ColliderModel/Box", "Box.obj");
}

void OBBColliderModel::Update()
{
	world_.Update();
}

void OBBColliderModel::Draw()
{
	model->WireFrameDraw(world_);
}
