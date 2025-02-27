#include "PDead.h"
#include "../../Player.h"
#include "PostProsess/RGBshift/renderTextrue/RGBshift.h"
PDead::PDead()
{
	stateType = PlayerState::kDead;
	//パーティクル初期化
	deadParticle_ = std::make_unique<ParticleSystem>();
	deadParticle_->Init("project/resources/circle2.dds");
	deadParticle_->UpdateFunc = [this](Particle& particle) {return UpdatedeadParticle(particle); };
	//エミッター設定
	deadParticleEmitter.count = 5;
	deadParticleEmitter.frequency = 0.5f;
	deadParticleEmitter.particleRadius = { 0.5f,0.5f,1.0f };
	deadParticleEmitter.color = { 0.0f,0.0f,0.0f };
	deadParticleEmitter.speed = { 1.0f,1.0f,1.0f };
	//アニメーション設定
	deadAnimation = std::make_unique<Animation>();
	deadAnimation.reset(Animation::LoadAnimationFile("project/resources/Player", "player_dead.gltf"));
	deadAnimation->Init();
	RGBshift::isEnableFlag = true;

}
void PDead::Init(Player* p)
{
	deadParticleEmitter.world_.transform.translate = p->GetWorld().transform.translate;
	deadParticleEmitter.world_.transform.translate.y += 1.0f;
	deadParticleEmitter.frequencyTime = deadParticleEmitter.frequency;
	animationTime_ = kDeltaTime;
	//アニメーション設定
	deadAnimation->AnimeInit(*p->Getmodels()[Player::PlayerModel::MainBody], true);
	if (p->GetCauseOfDeath() == Player::CauseOfDeath::Slam) {
		Input::VibrateController(1024, VIBRATION_MIN,10.0f);
	}
	else {
		Input::VibrateController(1024, 1024,10.0f);
	}
}
void PDead::Update(Player* p)
{
	if (p->GetCauseOfDeath() == Player::CauseOfDeath::Normal) {
		NormalDeadUpdate(p);
	}
	if (p->GetCauseOfDeath() == Player::CauseOfDeath::Slam) {
		SlamDeadUpdate(p);
	}
}
void PDead::Draw(Player* p)
{
	if (isModelDraw) {
		p->Getmodels()[Player::PlayerModel::MainBody]->RendererSkinDraw(p->GetWorld(), deadAnimation->GetSkinCluster());
	}

	deadParticle_->RendererDraw();
}
std::string PDead::ShowState()
{
	return "PDead";
}
void PDead::NormalDeadUpdate(Player* p)
{
	//死亡アニメーション更新
	animationTime_ += kDeltaTime;
	deadAnimation->PlayAnimation(false);
	if (animationTime_ >= deadAnimation->duration) {
		animationTime_ = 0.0f;
		isModelDraw = false;
		p->isDead = true;
		RGBshift::isEnableFlag = false;
	}
	if (isModelDraw == false) {
		//パーティクル生成
		ParticleSystem::ParticleSpawn(*deadParticle_, deadParticleEmitter);
		deadParticle_->Update();
	}
}
void PDead::SlamDeadUpdate(Player* p)
{
	//モデルをペシャンコにする
	p->GetWorld().transform.scale.y = 0.3f;
	//死亡アニメーション更新
	animationTime_ += kDeltaTime;

	if (animationTime_ >= deadAnimation->duration) {
		p->isDead = true;
		RGBshift::isEnableFlag = false;
	}

}
void PDead::UpdatedeadParticle(Particle& particle)
{
	//速度を1/60にする
	Vector3 velcity = particle.velocity * kDeltaTime;
	particle.transform.translate += velcity * deadParticleEmitter.speed;
	//エミッターがパーティクルの半径を決める
	particle.transform.scale = deadParticleEmitter.particleRadius;
	Vector3 translate = particle.transform.translate;
	float alpha = 1.0f - (particle.currentTime / particle.lifeTime);
	//色をセット
	particle.color.w = alpha;
	particle.color.x = deadParticleEmitter.color.x;
	particle.color.y = deadParticleEmitter.color.y;
	particle.color.z = deadParticleEmitter.color.z;
	particle.currentTime += kDeltaTime;
	particle.matWorld = MakeAffineMatrix(particle.transform.scale, Vector3{ 0.0f,0.0f,0.0f }, translate);
}