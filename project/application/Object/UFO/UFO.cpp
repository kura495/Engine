#include "UFO.h"

void UFO::Init()
{
	//3Dモデル
	UFOmodel_ = std::make_unique<Model>();
	UFOmodel_.reset(Model::CreateModelFromObj("project/resources/UFO", "UFO.obj"));

	UFOWorld_.Init();
	UFOWorld_.transform.translate.z = 10.0f;
	UFOWorld_.Update();

	ChangeState<Normal>();
}

void UFO::Update()
{
	state_->Update(this);

	if (Input::GetPadTrreger().x == 255 && Input::GetPadTrreger().y == 255) {
		if (state_.get()->GetStateType() != UFOState::Boost) {
			ChangeState<Boost>();
		}
	}
	else if(Input::GetPadTrreger().x >= 1 && Input::GetPadTrreger().y >= 1) {
		if (state_.get()->GetStateType() != UFOState::Normal) {
			ChangeState<Normal>();
		}
	}
	else if (Input::GetPadTrreger().x == 0 && Input::GetPadTrreger().y == 0) {
		if (state_.get()->GetStateType() != UFOState::Idle) {
			ChangeState<Idle>();
		}
	}
}

void UFO::Draw()
{
	UFOmodel_->RendererDraw(UFOWorld_);

	state_->Draw(this);
}
