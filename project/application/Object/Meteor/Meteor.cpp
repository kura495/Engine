#include "Meteor.h"

void Meteor::Init()
{
	model_ = std::make_unique<Model>();
	model_.reset(Model::CreateModelFromObj("project/resources/Meteor", "Meteor.obj"));
	world_.Init();
	world_.transform.translate = {random::Generate<float>(-5.0f,5.0f),random::Generate<float>(-5.0f,5.0f),random::Generate<float>(-5.0f,5.0f) };

}

void Meteor::Update()
{
}

void Meteor::Draw()
{
	model_->RendererDraw(world_);
}
