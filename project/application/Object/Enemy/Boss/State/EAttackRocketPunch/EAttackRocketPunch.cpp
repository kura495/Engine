#include "EAttackRocketPunch.h"
#include "../../Boss.h"
#include "application/Object/Player/Player.h"
void EAttackRocketPunch::Init(Boss* boss)
{
	boss->SetColliderAttribute(Boss::ColliderType::Arm, Collider::Tag::Enemy);
	boss->SetColliderAttribute(Boss::ColliderType::Hund, Collider::Tag::Enemy);

	easeT = 0.0f;
	PrePos = boss->GetWorld().transform.translate;
	addEaseT = 0.02f;
}
void EAttackRocketPunch::Update(Boss* boss)
{
	if (modeFlag == mode::Preparation) {
		PreparationFunc(boss);

		if (easeT == 1.0f) {
			//条件でモード移行
			modeFlag = mode::Attack;
			easeT = 0.0f;
			playerToEnemy = playerToEnemy.Normalize();
		}
	}
	if (modeFlag == mode::Attack) {
		AttackFunc(boss);
		boss->SetColliderAttribute(Boss::ColliderType::Arm, Collider::Tag::EnemyAttack);
		boss->SetColliderAttribute(Boss::ColliderType::Hund, Collider::Tag::EnemyAttack);
		addEaseT = 0.05f;
		if (boss->GetWorld().transform.translate.y <= 0.0f) {
			boss->GetWorld().transform.translate.y = 0.0f;
		}
		if (easeT == 1.0f || boss->GetWorld().transform.translate.z < -25.0f) {
			modeFlag = mode::Stay;
			easeT = 0.0f;
		}
	}
	if (modeFlag == mode::Stay) {
		addEaseT = 0.02f;
		//当たり判定を通常に変更
		boss->SetColliderAttribute(Boss::ColliderType::Arm, Collider::Tag::Enemy);
		boss->SetColliderAttribute(Boss::ColliderType::Hund, Collider::Tag::Enemy);
		easeT = (std::min)(easeT + addEaseT, 1.0f);
		if (easeT == 1.0f) {
			boss->ChangeState<EReturnPosition>();
		}
	}

#ifdef _DEBUG
	ImGui::Begin("BossRocketPunch");
	ImGui::SliderFloat("addEaseT",&addEaseT,0.01f,1.0f);
	if (ImGui::Button("ReStert")) {
		easeT = 0.0f;
		modeFlag = mode::Preparation;
	}
	if (ImGui::Button("Attack")) {
		easeT = 0.0f;
		modeFlag = mode::Attack;
	}
	ImGui::End();
#endif
}
void EAttackRocketPunch::Draw(Boss* boss)
{
	boss->Getmodels()[Boss::BossModel::MainBody]->RendererSkinDraw(boss->GetWorld(), boss->GetAnime()->GetSkinCluster());
}
std::string EAttackRocketPunch::ShowState()
{
	return "ERocketPunch";
}
void EAttackRocketPunch::OnCollisionAttack(Boss* boss, const ICollider& collider)
{
	boss;
	if (collider.GetcollitionAttribute() == Collider::Tag::Floor) {
		if (modeFlag == mode::Attack) {
			easeT = (std::min)(easeT + addEaseT, 1.0f);
		}

	}
}
void EAttackRocketPunch::PreparationFunc(Boss* boss)
{
	easeT = (std::min)(easeT + addEaseT, 1.0f);


	Vector3 ramdomTranslate = { random::Generate<float>(-1.0f, 1.0f),random::Generate<float>(-1.0f, 1.0f) ,random::Generate<float>(-1.0f, 1.0f) };
	//ランダムに動かして揺らす
	boss->GetWorld().transform.translate = PrePos + ramdomTranslate;

	//方向を決める
	playerToEnemy = boss->GetPlayer()->GetWorld().transform.translate - boss->GetWorld().transform.translate;
	//移動ベクトルをカメラの角度だけ回転
	//追従対象からロックオン対象へのベクトル
	forTargetVector = playerToEnemy.Normalize();
	//プレイヤーの現在の向き
	Vector3 cross = Vector3::Normalize(Vector3::Cross({ 0.0f,0.0f,-1.0f }, forTargetVector));
	float dot = Vector3::Dot({ 0.0f,0.0f,-1.0f }, forTargetVector);
	//行きたい方向のQuaternionの作成
	Quaternion qua = Quaternion::MakeRotateAxisAngleQuaternion(cross, std::acos(dot));
	boss->GetWorld().transform.quaternion = Quaternion::Slerp(Quaternion::IdentityQuaternion(), qua,easeT);
}
void EAttackRocketPunch::AttackFunc(Boss* boss)
{
	boss->GetWorld().transform.translate = boss->GetWorld().transform.translate + (playerToEnemy * speedValue_);
	if (boss->GetWorld().transform.translate.y < 0) {
		boss->GetWorld().transform.translate.y = 0;
	}
	speedValue_ = (std::min)(speedValue_ + addSpeedValue_, 2.0f);

}
