#include "DummyBomb.h"

void DummyBomb::Init(std::vector<Model*> models)
{
	models_ = models;
	world_.Initialize();
	ColliderInit();

	particle_ = std::make_unique<ParticleSystem>();
	particle_->Initalize("resources/circle2.dds");
	particle_->UpdateParticle = [this](Particle& particle) { UpdateParticle(particle); };
	particle_->CustumSpawn = [this]() { return CustomParticle(); };

	emitter.count = 10;
	emitter.frequency = 0.1f;
	emitter.particleRadius = { 1.75f,1.75f,1.75f };
	emitter.color = { 1.0f,1.0f,1.0f };
	emitter.speed = { 2.0f,2.0f,2.0f };

}


void DummyBomb::Update()
{
	//パーティクル
	emitter.frequencyTime += kDeltaTime;
	if (isThrowFlag) {
		if (emitter.frequency <= emitter.frequencyTime) {
			//ランダム生成用
			std::random_device seedGenerator;
			std::mt19937 randomEngine(seedGenerator());

			particle_->CustumSpawnParticle(emitter);

			emitter.frequencyTime -= emitter.frequency;
		}
	}

	particle_->Update();
	ImGui();


	//一定ラインを超えたら止める
	distance = Vector3::Distance(stertPos, world_.transform.translate);
	if (distance >= 60.0f) {
		IsOverline = true;
	}
	else {
		IsOverline = false;
	}

	if (IsOverline) {
		return;
	}

	//移動の制限(下限と上限を一行で書いている)
	world_.transform.translate.y = (std::max)(world_.transform.translate.y, 1.0f);

	if (isThrowFlag) {
		if (isCompleteMoveFlag == false) {
			easeT = (std::min)(easeT + addEaseT, 1.0f);
			world_.transform.translate = Vector3::Lerp(startPoint, targetPoint, easeT);
			if (easeT == 1.0f) {
				isCompleteMoveFlag = true;
				easeT = 0.0f;
			}
		}
		else {
			world_.transform.translate += forTargetVector;

		}
	}

	world_.Update();
}

void DummyBomb::Draw()
{
	if (collider.IsUsing) {
		particle_->RendererDraw();

	}
	//models_[0]->RendererDraw(world_);
}

void DummyBomb::ColliderInit()
{
	collider.Init(&world_);
	collider.SetSize({ 1.0f,1.0f,1.0f });
	collider.OnCollision = [this](ICollider& colliderA) { OnCollision(colliderA); };
	collider.SetcollitionAttribute(ColliderTag::EnemyBomb);
	collider.IsUsing = false;
}

void DummyBomb::OnCollision(const ICollider& colliderA)
{
	if (colliderA.GetcollitionAttribute() == ColliderTag::Weapon) {
		collider.IsUsing = false;
	}
}

void DummyBomb::UpdateParticle(Particle& particle)
{
	Vector3 velcity = particle.velocity * kDeltaTime;
	//エミッターがパーティクルの半径を決める
	particle.transform.scale = emitter.particleRadius;
	particle.transform.translate = world_.transform.translate;
	float alpha = 1.0f - (particle.currentTime / particle.lifeTime);
	particle.color.w = alpha;
	particle.color.x = emitter.color.x;
	particle.color.y = emitter.color.y;
	particle.color.z = emitter.color.z;
	particle.currentTime += kDeltaTime;
	particle.matWorld = MakeAffineMatrix(particle.transform.scale, Vector3{ 0.0f,0.0f,0.0f }, particle.transform.translate);
}

Particle DummyBomb::CustomParticle()
{
	Particle particle{};

	particle.color = { 0.5f,0.5f,1.0f };
	particle.currentTime = 0.0f;
	particle.lifeTime = 2.0f;
	particle.transform.translate = world_.transform.translate;
	particle.transform.scale = emitter.particleRadius;


	return particle;
}

void DummyBomb::ImGui()
{
#ifdef _DEBUG
	ImGui::Begin("Bomb");
	ImGui::InputFloat("z", &world_.transform.translate.z);
	ImGui::InputFloat("length", &distance);
	if (IsOverline) {
		ImGui::Text("IsOverline");
	}

	ImGui::End();
#endif
}
