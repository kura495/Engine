#include "EAttackSlam.h"
#include "../../Boss.h"
void EAttackSlam::Init(Boss* boss)
{
	addEaseT = 0.02f;
	boss->SetColliderUse(Boss::ColliderType::Arm,true);
	boss->SetColliderUse(Boss::ColliderType::Hund,true);
	easeT = 0.0f;
	//当たり判定を攻撃に変更
	boss->SetColliderAttribute(Boss::ColliderType::Arm,Collider::Tag::EnemyAttackSlam);
	boss->SetColliderAttribute(Boss::ColliderType::Hund,Collider::Tag::EnemyAttackFront | Collider::Tag::EnemyAttackSlam);

	SEHitattack = Audio::LoadAudioMP3("project/resources/sound/Boss/attackPlayer.mp3", false);
}
void EAttackSlam::Update(Boss* boss)
{
	if (IsAttackFlag) {
		easeT = (std::min)(easeT + addEaseT, 1.0f);

		boss->GetWorld().transform.translate.y -= Ease::InBack(easeT);
		float newPoint = Ease::InBack(easeT);

		if (newPoint > 0) {
			addEaseT = 0.06f;
		}
		//位置が0になったら
		if (boss->GetWorld().transform.translate.y <= 0) {
			boss->isSlamFlag = true;
			addEaseT = 0.01f;
			boss->SetColliderUse(Boss::ColliderType::Arm, false);
			boss->SetColliderUse(Boss::ColliderType::Hund, false);
			boss->GetWorld().transform.translate.y = 0.5f;
			easeT = 0.0f;
			IsAttackFlag = false;
			//初期位置に戻す
			boss->ChangeState<EReturnPosition>();
		}
	}
	else if (FollowPlayer(boss)) {
		IsAttackFlag = true;
	}
}
void EAttackSlam::Draw(Boss* boss)
{
	boss->Getmodels()[Boss::BossModel::MainBody]->RendererSkinDraw(boss->GetWorld(), boss->GetAnime()->GetSkinCluster());
}
void EAttackSlam::OnCollisionAttack(Boss* boss,const ICollider& collider)
{
	if (collider.GetcollitionAttribute() == Collider::Tag::Player) {
		boss->SetColliderUse(Boss::ColliderType::Arm,false);
		boss->SetColliderUse(Boss::ColliderType::Hund,false);
		Audio::Play(SEHitattack, 1.0f);
	}
}
std::string EAttackSlam::ShowState()
{
	return "EAttackSlam";
}
bool EAttackSlam::FollowPlayer(Boss* boss)
{
	Vector3 temp = boss->GetPlayer()->GetWorld().transform.translate - boss->GetWorld().transform.translate;
	//モデルの中心から手のひらへ
	temp.z += 5.0f;
	temp.y = 0.0f;
	float playerToEnemyLngth = temp.Length();
	temp = temp.Normalize();
	boss->GetWorld().transform.translate += temp * kFollowPlayerSpeed;
	//一定の距離になったら処理を終わる
	if (playerToEnemyLngth <= kConstantDistance) {
		return true;
	}
	return false;
}