#include "EAttackThrowball.h"
#include "../../Boss.h"
#include "application/Object/Player/Player.h"
void EAttackThrowball::Init(Boss* boss)
{
	easeT = 0.0f;
	addEaseT = 0.05f;

	ball = std::make_unique<Ball>();
	std::vector<Model*> ballmodels;
	ballmodels.push_back(Model::CreateModelFromObj("project/resources/Cube", "Cube.obj"));
	ball->Init(ballmodels);

	ball->ThrowBall(boss->GetWorld().transform.translate, boss->GetPlayer()->GetWorld().transform.translate);
	//音
	SEthrowBall = Audio::LoadAudioMP3("project/resources/sound/Boss/throwBall.mp3", false);
	Audio::Reset(SEthrowBall, false);
	Audio::Play(SEthrowBall, 0.2f);
	countHitBall = 0;

	boss->SetColliderUse(Boss::ColliderType::AttackArm, false);
	boss->SetColliderUse(Boss::ColliderType::AttackHund, false);

	ui_world_.Init();

	ui_ = std::make_unique<Sprite>();
	ui_->TextureHandle = TextureManager::GetInstance()->LoadTexture("project/resources/Attack.png");
	ui_->Initialize({ 0.0f,0.0f }, { 0.0f,720.0f }, { 1280.0f,0.0f }, { 1280.0f,720.0f });
}

void EAttackThrowball::Update(Boss* boss)
{
	easeT = (std::min)(easeT + kDeltaTime, 1.0f);
	if (easeT == 1.0f) {
		boss->SetColliderUse(Boss::ColliderType::AttackArm, true);
		boss->SetColliderUse(Boss::ColliderType::AttackHund, true);
	}

	ball->Update();
	//3回球に当たったらやられ状態にする
	if (countHitBall >= 3) {
		boss->HP_ -= 1;

		boss->isDamage = true;
		boss->ChangeState<ERoot>();
		return;
	}
	//ボールが一定のラインを超えたらルートビヘイビアーに戻す
	if (ball->GetIsOverline()) {
		boss->ChangeState<ERoot>();
	}
}

void EAttackThrowball::Draw(Boss* boss)
{
	boss->Getmodels()[Boss::BossModel::MainBody]->RendererDraw(boss->GetWorld());
	ball->Draw();

	ui_->RendererDraw(ui_world_);
}
void EAttackThrowball::OnCollisionAttack(Boss* boss, const ICollider& collider)
{
	if (collider.GetcollitionAttribute() == Collider::Tag::EnemyBall) {
		ball->Reset(boss->GetPlayer()->GetWorld().transform.translate);
		
		Audio::Stop(SEthrowBall, true, false);
		Audio::Play(SEthrowBall, 0.2f);

		countHitBall += 1;
		easeT = 0.0f;

		boss->SetColliderUse(Boss::ColliderType::AttackArm, false);
		boss->SetColliderUse(Boss::ColliderType::AttackHund, false);
	}
}
std::string EAttackThrowball::ShowState()
{
	return "EAttackThrowball";
}