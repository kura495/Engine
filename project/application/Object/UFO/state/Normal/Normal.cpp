#include "Normal.h"
#include "UFO/UFO.h"
void Normal::Init(UFO* ufo)
{
	ufo;
	//パーティクル初期化
	Particle_ = std::make_unique<ParticleSystem>();
	Particle_->Init("project/resources/circle2.dds");
	Particle_->CustumSpawnFunc = [this]() { return CustomParticle(); };
	Particle_->UpdateFunc = [this](Particle& particle) {return UpdateParticle(particle); };
	//エミッター設定
	ParticleEmitter.count = 5;
	ParticleEmitter.frequency = 0.2f;
	ParticleEmitter.frequencyTime = 0.2f;
	ParticleEmitter.particleRadius = { 0.2f,0.2f,1.0f };
	ParticleEmitter.speed = { 1.0f,1.0f,1.0f };
}
void Normal::Update(UFO* ufo)
{
	ufo->GetWorld().transform.translate = homePosition + random::Generate(-2.0f,2.0f);
	//パーティクル生成
	Particle_->CustumSpawnParticle(ParticleEmitter);
	Particle_->Update();

	if (Input::GetPadTrreger().x == 255 && Input::GetPadTrreger().y == 255) {
		ufo->ChangeState<Boost>();
	}
}
void Normal::Draw(UFO* ufo)
{
	ufo;
	Particle_->RendererDraw();
}
Particle Normal::CustomParticle()
{
	Particle particle{};

	particle.currentTime = 0.0f;
	particle.lifeTime = random::Generate<float>(0.0f,0.5f);

	particle.transform.translate = { random::Generate<float>(-0.7f,0.7f),random::Generate<float>(-0.5f,0.0f),10.0f};
	particle.transform.scale = ParticleEmitter.particleRadius;

	particle.color = { 0.7f,0.5f,0.5f,1.0f };

	return particle;
}
void Normal::UpdateParticle(Particle& particle)
{
	particle.transform.translate.z -= 0.1f;

	Vector3 translate = particle.transform.translate;
	float alpha = 1.0f - (particle.currentTime / particle.lifeTime);
	//色をセット
	ParticleEmitter.color.w = alpha;
	particle.currentTime += kDeltaTime;
	particle.matWorld = MakeAffineMatrix(particle.transform.scale, Vector3{ 0.0f,0.0f,0.0f }, translate);
}
