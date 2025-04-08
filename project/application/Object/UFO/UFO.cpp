#include "UFO.h"

void UFO::Init()
{
	//3Dモデル
	model_ = std::make_unique<Model>();
	model_.reset(Model::CreateModelFromObj("project/resources/UFO", "UFO.obj"));

	world_.Init();
	world_.transform.translate.z = 10.0f;
	world_.Update();

	ChangeState<Normal>();
}

void UFO::Update()
{
	state_->Update(this);
}

void UFO::Draw()
{
	model_->RendererDraw(world_);

	state_->Draw(this);
}
