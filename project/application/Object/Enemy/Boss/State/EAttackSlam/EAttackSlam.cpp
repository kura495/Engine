#include "EAttackSlam.h"
#include "../../Boss.h"
void EAttackSlam::Init(Boss* boss)
{
	addEaseT = 0.02f;
	boss->SetColliderUse(Boss::ColliderType::Arm,true);
	boss->SetColliderUse(Boss::ColliderType::Hund,true);
	boss->IsAttackFlag = true;
	easeT = 0.0f;
	//当たり判定を攻撃に変更
	boss->SetColliderAttribute(Boss::ColliderType::Arm,Collider::Tag::EnemyAttack);
	boss->SetColliderAttribute(Boss::ColliderType::Hund, Collider::Tag::EnemyAttack | Collider::Tag::EnemyAttackFront);
}

void EAttackSlam::Update(Boss* boss)
{
	if (boss->IsAttackFlag) {
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
			boss->IsAttackFlag = false;
		}
	}
	else {
		//2回目移行かつHPが低くなった時に処理を実行
		if (boss->isSlam2ndFlag && boss->GetHP() <= 3) {
			//falseにすることで2回連続で叩きつけをするようにする
			boss->isAttackSelect = false;
			boss->isSlam2ndFlag = false;
		}
		else if (boss->isSlam2ndFlag == false) {
			boss->isSlam2ndFlag = true;
		}
		//初期位置に戻す
		boss->ChangeState<EReturnPosition>();
	}
}

void EAttackSlam::Draw(Boss* boss)
{
	boss->Getmodels()[0]->RendererSkinDraw(boss->GetWorld(), boss->GetAnime()->GetSkinCluster());
}
std::string EAttackSlam::ShowState()
{
	return "EAttackSlam";
}