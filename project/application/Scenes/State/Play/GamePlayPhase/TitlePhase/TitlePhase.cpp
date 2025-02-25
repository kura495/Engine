#include "TitlePhase.h"
#include "State/Play/GamePlayState.h"
void TitlePhase::Init(GamePlayState* playState)
{
	playState;
	fade.Init();

	BGMHundle = Audio::LoadAudioMP3("project/resources/sound/BGM/Nisemono_Rock.mp3", true);
	Audio::Play(BGMHundle, audioValue);

	woodenBox.push_back(Model::CreateModelFromObj("project/resources/Box", "Box.gltf"));

	woodenBoxWorld_.Init();
	woodenBoxWorld_.transform.translate.z = -3.0f;
	woodenBoxWorld_.Update();

	tutorialModel.push_back(Model::CreateModelFromObj("project/resources/Tutorial", "Tutorial.obj"));
	tutorialWorld_.Init();
	tutorialWorld_.transform.translate.z = -3.0f;
	tutorialWorld_.transform.translate.x = -2.0f;
	tutorialWorld_.Update();
	stertCount = 0.0f;
	woodenBoxWorld_.transform.scale.x = stertCount + 1.0f;
	woodenBoxWorld_.transform.scale.y = stertCount + 1.0f;
	woodenBoxWorld_.transform.scale.z = stertCount + 1.0f;
	woodenBoxWorld_.Update();

	titleSprite = std::make_unique<Sprite>();
	titleSprite->Initialize({ 0.0f,0.0f }, { 0.0f,720.0f }, { 1280.0f,0.0f }, { 1280.0f,720.0f });
	titleSprite->TextureHandle = TextureManager::GetInstance()->LoadTexture("project/resources/Title.png");
	title.Init();
}

void TitlePhase::Update(GamePlayState* playState)
{
	audioValue = (std::min)(audioValue + 0.001f, kMaxaudioValue);
	Audio::Play(BGMHundle, audioValue);
	if (fade.Out(1.0f) == false) {
		return;
	}
	playState->followCamera->Update();
#ifdef _DEBUG
	ImGui::Begin("Title");
	ImGui::InputFloat("stertCount", &stertCount);
	ImGui::End();
#endif

	if (Input::GetPadPrecede(XINPUT_GAMEPAD_X, 20)) {
		stertCount += 0.3f;

	}
	woodenBoxWorld_.Update();
	stertCount = (std::max)(stertCount - subStertCount, 0.0f);

	woodenBoxWorld_.transform.scale.x = stertCount + 1.0f;
	woodenBoxWorld_.transform.scale.y = stertCount + 1.0f;
	woodenBoxWorld_.transform.scale.z = stertCount + 1.0f;

	if (stertCount >= 0.7f) {
		playState->ChangePhase<PlayPhase>();
	}
}

void TitlePhase::Draw(GamePlayState* playState)
{
	playState;
	titleSprite->RendererDraw(title);

	woodenBox[0]->RendererDraw(woodenBoxWorld_);
	tutorialModel[0]->RendererDraw(tutorialWorld_);

	fade.Draw();
}
