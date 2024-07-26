#include "Goal.h"

void Goal::Initialize(const std::vector<Model*> models)
{
	models_ = models;
	world_.Initialize();
	world_.transform.translate = { -22.0f,1.0f,16.0f };
	world_.UpdateMatrix();

	BoxCollider::Initialize(&world_);
	BoxCollider::SetSize({1.0f,1.0f,1.0f});
	BoxCollider::SetcollitionAttribute(ColliderTag::Goal);
	BoxCollider::SetcollisionMask(~ColliderTag::Goal);
	BoxCollider::OnCollision = [this](ICollider* collider) { OnCollision(collider); };
	animation = new Animation();
	animation = Animation::LoadAnimationFile("resources/Goal", "Goal.gltf");

	audio = Audio::GetInstance();
	audioHundle = audio->LoadAudioMP3("resources/powerup01.mp3",false);
}

void Goal::Update()
{

	animationTime_ += 1.0f / 60.0f;
	animationTime_ = std::fmod(animationTime_, animation->duration);

	NodeAnimation& rootNodeAnimation = animation->nodeAnimations[models_[0]->GetModelData().rootNode.name];
	Vector3 translate = Animation::CalculateValue(rootNodeAnimation.translate.keyFrames, animationTime_);
	Quaternion rotation = Animation::CalculateValue(rootNodeAnimation.rotate.keyFrames, animationTime_);
	Vector3 scale = Animation::CalculateValue(rootNodeAnimation.scale.keyFrames, animationTime_);
	world_.transform.translate += translate;
	world_.transform.quaternion += rotation;
	world_.transform.scale = scale;
	world_.UpdateMatrix();

}

void Goal::Draw()
{
	for (Model* model : models_) {
		model->Draw(world_);
	}
}

void Goal::ImGui()
{
}

void Goal::OnCollision(const ICollider* collider)
{
	if (collider->GetcollitionAttribute()) {
		audio->Play(audioHundle, 0.5f);
		return;
	}
}

