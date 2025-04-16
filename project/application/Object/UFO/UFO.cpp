#include "UFO.h"

void UFO::Init()
{
	//3Dモデル
	model_ = std::make_unique<Model>();
	model_.reset(Model::CreateModelFromObj("project/resources/UFO", "UFO.obj"));

	world_.Init();

	ChangeState<Idle>();

	particle_ = std::make_unique<ParticleSystem>();
	particle_->Init("project/resources/circle2.dds");
	particle_->CustumSpawnFunc = [this]() { return CustomParticle(); };
	particle_->UpdateFunc = [this](Particle& particle) {return UpdateParticle(particle); };


}

void UFO::Update()
{
	state_->Update(this);

	world_.Update();
}

void UFO::Draw()
{
	model_->RendererDraw(world_);
	particle_->RendererDraw();
	state_->Draw(this);
}

void UFO::ImGui()
{
#ifdef _DEBUG
	ImGui::Begin("UFO");
	ImGui::DragFloat3("world",&world_.transform.translate.x);
	ImGui::End();
#endif
}

Particle UFO::CustomParticle()
{
	Particle particle{};

	particle.currentTime = 0.0f;
	particle.lifeTime = random::Generate<float>(0.0f, 0.5f);

	particle.transform.translate = { random::Generate<float>(-0.7f,0.7f),random::Generate<float>(-0.5f,0.0f),10.0f };
	particle.transform.scale = particleEmitter.particleRadius;

	particle.color = { 0.7f,0.5f,0.5f,1.0f };

	return particle;
}
void UFO::UpdateParticle(Particle& particle)
{
	particle.transform.translate.z -= 0.1f;

	Vector3 translate = particle.transform.translate;
	float alpha = 1.0f - (particle.currentTime / particle.lifeTime);
	//色をセット
	particleEmitter.color.w = alpha;
	particle.currentTime += kDeltaTime;
	particle.matWorld = MakeAffineMatrix(particle.transform.scale, Vector3{ 0.0f,0.0f,0.0f }, translate);
}