#include "PAttack.h"
#include "../../Player.h"
PAttack::PAttack()
{
	stateType = PlayerState::kAttack;
	//音声
	SEattack = Audio::LoadAudioMP3("project/resources/sound/Player/attack.mp3", false);
	SEHitattack = Audio::LoadAudioMP3("project/resources/sound/Player/Hitattack.mp3", false);
	attackAnimation = new Animation();
	attackAnimation = Animation::LoadAnimationFile("project/resources/Player", "player_attack.gltf");
	attackAnimation->Init();
	attackPosture = Quaternion::IdentityQuaternion();
}
void PAttack::Init(Player* p)
{
	attackAnimation->AnimeInit(*p->Getmodels()[0], true);
	p->colliderAttack.IsUsing = true;
	p->animationTime_ = 0.0f;
	attackPosture = p->GetWorld().transform.quaternion;
	p->isMovedFlag = false;
	Audio::Play(SEattack, 0.5f);
	Audio::Stop(SEHitattack, true, false);
}

void PAttack::Update(Player* p)
{
	//攻撃中も移動できるように
	p->Move();
	//攻撃中は一定の方向を向くように固定
	p->GetWorld().transform.quaternion = attackPosture;
	//アニメーション再生
	attackAnimation->PlayAnimation();
	p->animationTime_ += 1.0f / 60.0f;
	//重力
	p->GetWorld().transform.translate.y -= p->gravity;

	if (p->animationTime_ > attackAnimation->duration) {
		p->animationTime_ = 0.0f;
		p->colliderAttack.IsUsing = false;

		attackAnimation->Reset();
		Audio::Stop(SEattack, true, false);		
		//kRootに戻す
		p->ChangeState<PRoot>();
	}
}

void PAttack::Draw(Player* p)
{
	p->Getmodels()[0]->RendererSkinDraw(p->GetWorld(), attackAnimation->GetSkinCluster());
}

std::string PAttack::ShowState()
{
	return "Attack";
}
