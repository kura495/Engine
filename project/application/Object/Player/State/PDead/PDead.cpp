#include "PDead.h"
#include "../../Player.h"
#include "PostProsess/RGBshift/renderTextrue/RGBshift.h"
PDead::PDead()
{
	stateType = PlayerState::Dead;
	//パーティクル初期化
	deadParticle_ = std::make_unique<ParticleSystem>();
	deadParticle_->Init("project/resources/circle2.dds");
	deadParticle_->CustumSpawnFunc = [this]() { return CustomParticle(); };
	deadParticle_->UpdateFunc = [this](Particle& particle) {return UpdatedeadParticle(particle); };
	//エミッター設定
	deadParticleEmitter.count = 10;
	deadParticleEmitter.frequency = 0.2f;
	deadParticleEmitter.frequencyTime = 0.2f;
	deadParticleEmitter.particleRadius = { 0.2f,0.2f,1.0f };
	deadParticleEmitter.speed = { 1.0f,1.0f,1.0f };
	//アニメーション設定
	deadAnimation = std::make_unique<Animation>();
	deadAnimation.reset(Animation::LoadAnimationFile("project/resources/Player", "player_dead.gltf"));
	deadAnimation->Init();
	RGBshift::isEnableFlag = true;
	//再生位置を合わせるために1フレーム分だけ加算
	animationTime_ += kDeltaTime;
}
void PDead::Init(Player* player)
{
	deadParticleEmitter.world_.transform.translate = player->GetWorld().transform.translate;
	deadParticleEmitter.world_.transform.translate.y += 1.0f;
	//アニメーション設定
	deadAnimation->AnimeInit(*player->Getmodels()[Player::PlayerModel::MainBody], true);
	if (player->GetCauseOfDeath() == Player::CauseOfDeath::Slam) {
		Input::VibrateController(VIBRATION_MAX, VIBRATION_MIN,0.5f);
	}
	else {
		Input::VibrateController(20480, 20480,0.5f);
	}
}
void PDead::Update(Player* player)
{
#ifdef _DEBUG
	ImGui::Begin("PDead");
	ImGui::DragFloat3("Trans",&deadParticleEmitter.world_.transform.translate.x);
	ImGui::End();
#endif
	if (player->GetCauseOfDeath() == Player::CauseOfDeath::Normal) {
		NormalDeadUpdate(player);
	}
	if (player->GetCauseOfDeath() == Player::CauseOfDeath::Slam) {
		SlamDeadUpdate(player);
	}
}
void PDead::Draw(Player* player)
{
	player;
	if (isModelDraw) {
		player->Getmodels()[Player::PlayerModel::MainBody]->RendererSkinDraw(player->GetWorld(), deadAnimation->GetSkinCluster());
	}
	deadParticle_->RendererDraw();
}
std::string PDead::ShowState()
{
	return "PDead";
}
void PDead::NormalDeadUpdate(Player* player)
{
	//死亡アニメーション更新
	animationTime_ += kDeltaTime;
	deadAnimation->PlayAnimation();
	if (animationTime_ >= deadAnimation->duration) {
		isModelDraw = false;

		RGBshift::isEnableFlag = false;
	}
	if (isModelDraw == false) {
		easeT = (std::min)(easeT + 0.04f, 1.0f);
		//パーティクル生成
		deadParticle_->CustumSpawnParticle(deadParticleEmitter);
		deadParticle_->Update();
		if (easeT == 1.0f) {
			player->isDead = true;
		}
	}
}
void PDead::SlamDeadUpdate(Player* player)
{
	//モデルをペシャンコにする
	player->GetWorld().transform.scale.y = 0.3f;
	//死亡アニメーション更新
	animationTime_ += kDeltaTime;
	if (animationTime_ >= deadAnimation->duration) {
		player->isDead = true;
		RGBshift::isEnableFlag = false;
	}
}
Particle PDead::CustomParticle()
{
	Particle particle{};
	
	particle.currentTime = 0.0f;
	particle.lifeTime = 2.0f;
	Vector3 randomValue;
	randomValue.x = random::Generate<float>(-0.1f,0.1f);
	randomValue.y = random::Generate<float>(-0.1f,0.1f);
	randomValue.z = random::Generate<float>(-0.1f,0.1f);
	particle.transform.translate = deadParticleEmitter.world_.transform.translate + randomValue;
	particle.transform.scale = deadParticleEmitter.particleRadius;
	//移動量を決める
	randomValue.x = random::Generate<float>(-1.0f, 1.0f);
	randomValue.y = random::Generate<float>(-1.0f, 1.0f);
	randomValue.z = random::Generate<float>(-1.0f, 1.0f);
	particle.velocity = randomValue;

	randomValue.x = random::Generate<float>(0.5f, 1.0f);
	randomValue.y = random::Generate<float>(0.0f, 0.3f);
	randomValue.z = random::Generate<float>(0.0f, 0.3f);
	particle.color = { randomValue.x,randomValue.y,randomValue.z,1.0f };

	return particle;
}
void PDead::UpdatedeadParticle(Particle& particle)
{
	//速度を1/60にする
	Vector3 velcity = particle.velocity * kDeltaTime;
	particle.transform.translate += velcity * deadParticleEmitter.speed;
	//エミッターがパーティクルの半径を決める
	particle.transform.scale = deadParticleEmitter.particleRadius;
	Vector3 translate = particle.transform.translate;
	float alpha = 1.0f - easeT;
	//色をセット
	particle.color.w = alpha;
	particle.currentTime += kDeltaTime;
	particle.matWorld = MakeAffineMatrix(particle.transform.scale, Vector3{ 0.0f,0.0f,0.0f }, translate);
}