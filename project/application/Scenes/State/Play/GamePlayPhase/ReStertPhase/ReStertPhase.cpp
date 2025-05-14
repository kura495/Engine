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
	easeT = (std::min)(easeT + kDeltaTime, 1.0f);

	playState->GetCollisionManager()->Update();

	if (phase == Phase::FadeIn) {
		if (fade.In(1.0f)) {
			phase = Phase::ReStertFanc;
			easeT = 0.0f;
		}
	}
	else if (phase == Phase::ReStertFanc) {
		if (life_UI->Animation()) {
			playState->ReStert();
			phase = Phase::FadeOut;
			fade.OutInit("project/resources/BlackTexture.png");
		}
	}
	else if (phase == Phase::FadeOut) {
		if (fade.Out(1.0f)) {
			playState->ChangePhase<PlayPhase>();
		}
	}
}

void ReStertPhase::Draw(GamePlayState* playState)
{
	playState->GetPlayer()->Draw();
	fade.Draw();
	if (phase == Phase::ReStertFanc) {
		life_UI->Draw();
	}
}
