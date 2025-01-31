#include "EAttackThrowball.h"
#include "../../Boss.h"
#include "application/Object/Player/Player.h"
void EAttackThrowball::Init(Boss* Boss)
{
	easeT = 0.0f;
	addEaseT = 0.05f;
	Boss->ball->ThrowBall(Boss->GetWorld().transform.translate, Boss->GetPlayer()->GetWorld().transform.translate);
	Audio::Play(Boss->SEthrowBall, 0.2f);
	Boss->isThrowdummyBallFlag = false;
	Boss->countHitBall = 0;
}

void EAttackThrowball::Update(Boss* Boss)
{
	easeT = (std::min)(easeT + addEaseT, 1.0f);
	Boss->ball->Update();
	Boss->dummyBall->Update();
	//跳ね返しのタイミングでもう一つの球を出す
	if (Boss->countHitBall == 1 && Boss->isThrowdummyBallFlag == false) {
		Boss->dummyBall->ThrowBall(Boss->GetWorld().transform.translate, { Boss->GetWorld().transform.translate.x + 5.0f,Boss->GetWorld().transform.translate.y,Boss->GetWorld().transform.translate.z}, Boss->GetPlayer()->GetWorld().transform.translate);
		Boss->isThrowdummyBallFlag = true;
	}
	//3回球に当たったらやられ状態にする
	if (Boss->countHitBall >= 3) {
		Boss->dummyBall->Reset();
		Boss->ChangeState<EDown>();
	}
	//ボールが一定のラインを超えたらルートビヘイビアーに戻す
	if (Boss->ball->GetIsOverline()) {
		Boss->dummyBall->Reset();
		Boss->ChangeState<ERoot>();
	}
}

void EAttackThrowball::Draw(Boss* Boss)
{
	Boss->Getmodels()[0]->RendererSkinDraw(Boss->GetWorld(), Boss->GetAnime()->GetSkinCluster());
	Boss->ball->Draw();
	Boss->dummyBall->Draw();
}
std::string EAttackThrowball::ShowState()
{
	return "EAttackThrowball";
}