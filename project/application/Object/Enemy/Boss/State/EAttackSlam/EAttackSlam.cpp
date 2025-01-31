#include "EAttackSlam.h"
#include "../../Boss.h"
void EAttackSlam::Init(Boss* Boss)
{
	addEaseT = 0.02f;
	Boss->SetColliderUse(ColliderNumber::Arm,true);
	Boss->SetColliderUse(ColliderNumber::Hund,true);
	Boss->IsAttackFlag = true;
	easeT = 0.0f;
	//当たり判定を攻撃に変更
	Boss->SetColliderAttribute(ColliderNumber::Arm,ColliderTag::EnemyAttack);
	Boss->SetColliderAttribute(ColliderNumber::Hund, ColliderTag::EnemyAttack | ColliderTag::EnemyAttackFront);
}

void EAttackSlam::Update(Boss* Boss)
{
	if (Boss->IsAttackFlag) {
		easeT = (std::min)(easeT + addEaseT, 1.0f);

		Boss->GetWorld().transform.translate.y -= Ease::InBack(easeT);
		float newPoint = Ease::InBack(easeT);

		if (newPoint > 0) {
			addEaseT = 0.06f;
		}
		//位置が0になったら
		if (Boss->GetWorld().transform.translate.y <= 0) {
			Boss->isSlamFlag = true;
			addEaseT = 0.01f;
			Boss->SetColliderUse(ColliderNumber::Arm, false);
			Boss->SetColliderUse(ColliderNumber::Hund, false);
			Boss->GetWorld().transform.translate.y = 0.5f;
			easeT = 0.0f;
			Boss->IsAttackFlag = false;
		}
	}
	else {
		//2回目移行かつHPが低くなった時に処理を実行
		if (Boss->isSlam2ndFlag && Boss->GetHP() <= 3) {
			//falseにすることで2回連続で叩きつけをするようにする
			Boss->isAttackSelect = false;
			Boss->isSlam2ndFlag = false;
		}
		else if (Boss->isSlam2ndFlag == false) {
			Boss->isSlam2ndFlag = true;
		}
		//初期位置に戻す
		Boss->ChangeState<EReturnPosition>();
	}
}

void EAttackSlam::Draw(Boss* Boss)
{
	Boss->Getmodels()[0]->RendererSkinDraw(Boss->GetWorld(), Boss->GetAnime()->GetSkinCluster());
}
std::string EAttackSlam::ShowState()
{
	return "EAttackSlam";
}