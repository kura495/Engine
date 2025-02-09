#include "EAttackThrowball.h"
#include "../../Boss.h"
#include "application/Object/Player/Player.h"
void EAttackThrowball::Init(Boss* boss)
{
	easeT = 0.0f;
	addEaseT = 0.05f;
	boss->ball->ThrowBall(boss->GetWorld().transform.translate, boss->GetPlayer()->GetWorld().transform.translate);
	Audio::Reset(boss->SEthrowBall, false);
	Audio::Play(boss->SEthrowBall, 0.2f);
	boss->isThrowdummyBallFlag = false;
	boss->countHitBall = 0;
}

void EAttackThrowball::Update(Boss* boss)
{
	easeT = (std::min)(easeT + addEaseT, 1.0f);
	boss->ball->Update();
	boss->dummyBall->Update();
	//跳ね返しのタイミングでもう一つの球を出す
	if (boss->countHitBall == 1 && boss->isThrowdummyBallFlag == false) {
		boss->dummyBall->ThrowBall(boss->GetWorld().transform.translate, { boss->GetWorld().transform.translate.x + 5.0f,boss->GetWorld().transform.translate.y,boss->GetWorld().transform.translate.z}, boss->GetPlayer()->GetWorld().transform.translate);
		boss->isThrowdummyBallFlag = true;
	}
	//3回球に当たったらやられ状態にする
	if (boss->countHitBall >= 3) {
		boss->dummyBall->Reset();
		boss->ChangeState<EDown>();
	}
	//ボールが一定のラインを超えたらルートビヘイビアーに戻す
	if (boss->ball->GetIsOverline()) {
		boss->dummyBall->Reset();
		boss->ChangeState<ERoot>();
	}
}

void EAttackThrowball::Draw(Boss* boss)
{
	boss->Getmodels()[Boss::BossModel::MainBody]->RendererSkinDraw(boss->GetWorld(), boss->GetAnime()->GetSkinCluster());
	boss->ball->Draw();
	boss->dummyBall->Draw();
}
std::string EAttackThrowball::ShowState()
{
	return "EAttackThrowball";
}