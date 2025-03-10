#include "PAttack.h"
#include "../../Player.h"
PAttack::PAttack()
{
	stateType = PlayerState::Attack;
	//音声
	SEattack = Audio::LoadAudioMP3("project/resources/sound/Player/attack.mp3", false);
	SEHitattack = Audio::LoadAudioMP3("project/resources/sound/Player/Hitattack.mp3", false);
	attackAnimation = new Animation();
	attackAnimation = Animation::LoadAnimationFile("project/resources/Player", "player_attack.gltf");
	attackAnimation->Init();
	attackPosture = Quaternion::IdentityQuaternion();
}
void PAttack::Init(Player* player)
{
	attackAnimation->AnimeInit(*player->Getmodels()[Player::PlayerModel::MainBody], true);
	player->SetColliderUse(Player::ColliderType::Attack,true);
	animationTime_ = 0.0f;
	attackPosture = player->GetWorld().transform.quaternion;
	Audio::Play(SEattack, 0.5f);
	Audio::Stop(SEHitattack, true, false);
}

void PAttack::Update(Player* player)
{
	//攻撃中も移動できるように
	player->Move();
	//攻撃中は一定の方向を向くように固定
	player->GetWorld().transform.quaternion = attackPosture;
	//アニメーション再生
	attackAnimation->PlayAnimation();
	animationTime_ += kDeltaTime;

	if (animationTime_ > attackAnimation->duration) {
		animationTime_ = 0.0f;
		player->SetColliderUse(Player::ColliderType::Attack, false);

		attackAnimation->Reset();
		Audio::Stop(SEattack, true, false);		
		//kRootに戻す
		player->ChangeState<PRoot>();
	}
}

void PAttack::Draw(Player* player)
{
	player->Getmodels()[Player::PlayerModel::MainBody]->RendererSkinDraw(player->GetWorld(), attackAnimation->GetSkinCluster());
}

std::string PAttack::ShowState()
{
	return "Attack";
}
