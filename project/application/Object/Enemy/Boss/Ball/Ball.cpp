#include "Ball.h"

void Ball::Init(std::vector<Model*> models)
{
	models_ = models;
	world_.Init();
	ColliderInit();
	//パーティクル設定
	particle_ = std::make_unique<ParticleSystem>();
	particle_->Init("project/resources/circle2.dds");
	particle_->UpdateFunc = [this](Particle& particle) { UpdateParticle(particle); };
	particle_->CustumSpawnFunc = [this]() { return CustomParticle(); };
	//パーティクルエミッターの設定
	emitter.count = 10;
	emitter.frequency = 0.1f;
	emitter.particleRadius = { 1.75f,1.75f,1.75f };
	emitter.color = { 0.5f,0.5f,1.0f };
	emitter.speed = { 2.0f,2.0f,2.0f };

}

void Ball::Update()
{
	//パーティクル生成
	emitter.frequencyTime += kDeltaTime;
	if (emitter.frequency <= emitter.frequencyTime) {

		particle_->CustumSpawnParticle(emitter);

		emitter.frequencyTime -= emitter.frequency;
	}
	particle_->Update();
	ImGui();

	world_.Update();
	//一定ラインを超えたら止める
	distance = Vector3::Distance(stertPos, world_.transform.translate);
	if (distance >= 60.0f) {
		IsOverline = true;
		collider.IsUsing = false;
	}
	else {
		IsOverline = false;
	}

	if (IsOverline) {
		return;
	}
	//移動の制限(下限と上限を一行で書いている)
	world_.transform.translate.y = (std::max)(world_.transform.translate.y,1.0f);

	if (isThrowFlag) {
		world_.transform.translate += forTargetVector;
	}
}

void Ball::Draw()
{
	particle_->RendererDraw();
}

void Ball::ColliderInit()
{
	collider.Init(&world_);
	collider.SetSize({ 1.0f,1.0f,1.0f });
	collider.OnCollision = [this](ICollider& colliderA) { OnCollision(colliderA); };
	collider.SetcollitionAttribute(Collider::Tag::EnemyBall);
	collider.IsUsing = false;
}

void Ball::OnCollision(const ICollider& colliderA)
{
	if (colliderA.GetcollitionAttribute() == Collider::Tag::Weapon) {
		if (isHit) {
			//方向を決める
			Vector3 playerToBomb = stertPos - world_.transform.translate;
			//速さの定数を掛ける
			accelValue += 1.0f;
			forTargetVector = playerToBomb.Normalize() * kSpeedValue * accelValue;
			isHit = false;
		}
	}
	if (colliderA.GetcollitionAttribute() == Collider::Tag::EnemyCore){
		if (isHit == false) {
			forTargetVector *= -1.0f;
			isHit = true;
		}
	}
	if (colliderA.GetcollitionAttribute() == Collider::Tag::Player) {
		collider.IsUsing = false;
	}
}

void Ball::UpdateParticle(Particle& particle)
{
	Vector3 velcity = particle.velocity * kDeltaTime;
	//particle.transform.translate += velcity * emitter.speed;
	//エミッターがパーティクルの半径を決める
	particle.transform.scale = emitter.particleRadius;
	particle.transform.translate = world_.transform.translate;
	float alpha = 1.0f - (particle.currentTime / particle.lifeTime);
	emitter.color.w = alpha;
	particle.color = emitter.color;
	particle.currentTime += kDeltaTime;
	particle.matWorld = MakeAffineMatrix(particle.transform.scale, Vector3{ 0.0f,0.0f,0.0f }, particle.transform.translate);
}

Particle Ball::CustomParticle()
{
	Particle particle{};

	particle.color = { 0.5f,0.5f,1.0f };
	particle.currentTime = 0.0f;
	particle.lifeTime = 2.0f;
	particle.transform.translate = world_.transform.translate;
	particle.transform.scale = emitter.particleRadius;

	return particle;
}

void Ball::ImGui()
{
#ifdef _DEBUG
	ImGui::Begin("Bomb");
	ImGui::InputFloat("z",&world_.transform.translate.z);
	ImGui::InputFloat("length",&distance);
	if (IsOverline) {
		ImGui::Text("IsOverline" );
	}

	ImGui::End();
#endif
}
