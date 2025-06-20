#include "ReStertPhase.h"
#include "State/Play/GamePlayState.h"
void ReStertPhase::Init(GamePlayState* playState)
{
	playState;

	fade.InInit("project/resources/BlackTexture.png");

	life_UI = std::make_unique<Life_UI>();
	life_UI->Init(playState->GetPlayer()->GetHP());

}

void ReStertPhase::Update(GamePlayState* playState)
{

	playState->GetEnemyManager()->Update();
	playState->GetFollowCamera()->Update();
	playState->GetCollisionManager()->Update();
	if (phase == Phase::ReStertFanc) {
		if (life_UI->Animation()) {
			phase = Phase::FadeOut;
			fade.OutInit("project/resources/BlackTexture.png");
			playState->ReStert();
		}
	}
	else if (phase == Phase::FadeOut) {

		if (fade.Out(0.5f)) {

			playState->ChangePhase<PlayPhase>();
		}
	}
}

void ReStertPhase::Draw(GamePlayState* playState)
{
	playState->GetPlayer()->Draw();
	//TODO:画像を表示させない
	//fade.Draw();
	if (phase == Phase::ReStertFanc) {
		life_UI->Draw();
	}
}
