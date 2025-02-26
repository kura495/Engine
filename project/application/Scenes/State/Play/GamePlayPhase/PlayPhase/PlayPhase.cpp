#include "PlayPhase.h"
#include "State/Play/GamePlayState.h"
void PlayPhase::Init(GamePlayState* playState)
{
	playState->GetPlayer()->ChangeState<PJump>();
}

void PlayPhase::Update(GamePlayState* playState)
{
	playState->GetPlayer()->Update();
	playState->GetCollisionManager()->Update();
	playState->GetEnemyManager()->Update();
	playState->GetFollowCamera()->Update();
#ifdef _DEBUG
	ImGui::Begin("Input Vib Test");
	ImGui::DragFloat2("Value",&moterValue.x,1.0f,0.0f,VIBRATION_MAX);
	Input::VibrateController((int)moterValue.x, (int)moterValue.y);
	ImGui::End();
#endif // DEBUG



	if (playState->GetEnemyManager()->GetisClear()) {
		playState->ChangePhase<ClearPhase>();
	}
	if (playState->GetPlayer()->GetisDead()) {
		playState->ChangePhase<GameOverPhase>();
	}
}

void PlayPhase::Draw(GamePlayState* playState)
{
	playState->GetPlayer()->Draw();
}