#include "TitlePhase.h"
#include "State/Play/GamePlayState.h"
void TitlePhase::Init(GamePlayState* playState)
{
	playState;
	fade.OutInit("project/resources/BlackTexture.png");

	BGMHundle = Audio::LoadAudioMP3("project/resources/sound/BGM/Nisemono_Rock.mp3", true);
	Audio::Play(BGMHundle, audioValue);
}

void TitlePhase::Update(GamePlayState* playState)
{
	audioValue = (std::min)(audioValue + 0.001f, kMaxaudioValue);
	Audio::Play(BGMHundle, audioValue);
	if (fade.Out(1.0f) == false) {
		return;
	}
	playState->GetFollowCamera()->Update();
#ifdef _DEBUG
	ImGui::Begin("Title");
	ImGui::InputFloat("stertCount", &stertCount);
	ImGui::End();
#endif
	stertCount += 0.3f;

	if (stertCount >= 0.7f) {
		playState->ChangePhase<PlayPhase>();
	}
}

void TitlePhase::Draw(GamePlayState* playState)
{
	playState;

	fade.Draw();
}
