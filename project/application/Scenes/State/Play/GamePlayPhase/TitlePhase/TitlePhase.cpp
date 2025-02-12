#include "TitlePhase.h"
#include "State/Play/GamePlayState.h"
void TitlePhase::Init(GamePlayState* playState)
{
	playState;
}

void TitlePhase::Update(GamePlayState* playState)
{
	playState->audioValue = (std::min)(playState->audioValue + 0.001f, playState->kMaxaudioValue);
	Audio::Play(playState->BGMHundle, playState->audioValue);
	if (playState->fade.Out() == false) {
		return;
	}
	playState->followCamera->Update();
#ifdef _DEBUG
	ImGui::Begin("Title");
	ImGui::InputFloat("stertCount", &playState->stertCount);
	ImGui::End();
#endif

	if (Input::GetPadPrecede(XINPUT_GAMEPAD_X, 20)) {
		playState->stertCount += 0.3f;

	}
	playState->woodenBoxWorld_.Update();
	playState->stertCount = (std::max)(playState->stertCount - playState->subStertCount, 0.0f);

	playState->woodenBoxWorld_.transform.scale.x = playState->stertCount + 1.0f;
	playState->woodenBoxWorld_.transform.scale.y = playState->stertCount + 1.0f;
	playState->woodenBoxWorld_.transform.scale.z = playState->stertCount + 1.0f;

	if (playState->stertCount >= 0.7f) {
		playState->IsTitleToGameFlag = true;
	}
	if (playState->IsTitleToGameFlag) {
		playState->ChangePhase<PlayPhase>();
	}
}

void TitlePhase::Draw(GamePlayState* playState)
{
	playState->titleSprite->RendererDraw(playState->title);

	playState->woodenBox[0]->RendererDraw(playState->woodenBoxWorld_);
	playState->tutorialModel[0]->RendererDraw(playState->tutorialWorld_);

	playState->fade.Draw();
}
