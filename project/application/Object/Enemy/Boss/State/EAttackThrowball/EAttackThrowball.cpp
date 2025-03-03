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
	dummyBall = std::make_unique<DummyBall>();
	dummyBall->Init(ballmodels);

	ball->ThrowBall(boss->GetWorld().transform.translate, boss->GetPlayer()->GetWorld().transform.translate);
	//音
	SEthrowBall = Audio::LoadAudioMP3("project/resources/sound/Boss/throwBall.mp3", false);
	Audio::Reset(SEthrowBall, false);
	Audio::Play(SEthrowBall, 0.2f);
	isThrowdummyBallFlag = false;
	countHitBall = 0;

	boss->SetColliderUse(Boss::ColliderType::AttackArm, false);
	boss->SetColliderUse(Boss::ColliderType::AttackHund, false);
}

void EAttackThrowball::Update(Boss* boss)
{
	easeT = (std::min)(easeT + kDeltaTime, 1.0f);
	if (easeT == 1.0f) {
		boss->SetColliderUse(Boss::ColliderType::AttackArm, true);
		boss->SetColliderUse(Boss::ColliderType::AttackHund, true);
	}

	ball->Update();
	dummyBall->Update();
	//跳ね返しのタイミングでもう一つの球を出す
	if (countHitBall == 1 && isThrowdummyBallFlag == false) {
		dummyBall->ThrowBall(boss->GetWorld().transform.translate, { boss->GetWorld().transform.translate.x + 5.0f,boss->GetWorld().transform.translate.y,boss->GetWorld().transform.translate.z}, boss->GetPlayer()->GetWorld().transform.translate);
		isThrowdummyBallFlag = true;
	}
	//3回球に当たったらやられ状態にする
	if (countHitBall >= 3) {
		dummyBall->Reset();
		boss->ChangeState<EDown>();
		return;
	}
	//ボールが一定のラインを超えたらルートビヘイビアーに戻す
	if (ball->GetIsOverline()) {
		dummyBall->Reset();
		boss->ChangeState<ERoot>();
	}
}

void EAttackThrowball::Draw(Boss* boss)
{
	boss->Getmodels()[Boss::BossModel::MainBody]->RendererDraw(boss->GetWorld());
	ball->Draw();
	dummyBall->Draw();
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