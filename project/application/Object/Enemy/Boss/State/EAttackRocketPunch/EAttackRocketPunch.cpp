#include "EAttackRocketPunch.h"
#include "../../Boss.h"
#include "application/Object/Player/Player.h"
void EAttackRocketPunch::Init(Boss* boss)
{
	boss->SetColliderAttribute(Boss::ColliderType::Arm,Collider::Tag::EnemyAttack);
	boss->SetColliderAttribute(Boss::ColliderType::Hund,Collider::Tag::EnemyAttack);
	easeT = 0.0f;
	addEaseT = 0.05f;
	PrePos = boss->GetWorld().transform.translate;
}

void EAttackRocketPunch::Update(Boss* boss)
{
	//方向を決める
	Vector3 playerToBomb = boss->GetPlayer()->GetWorld().transform.translate - boss->GetWorld().transform.translate;

	//移動ベクトルをカメラの角度だけ回転
	//追従対象からロックオン対象へのベクトル
	forTargetVector = playerToBomb.Normalize();
	//プレイヤーの現在の向き

	Vector3 cross = Vector3::Normalize(Vector3::Cross({ 0.0f,0.0f,-1.0f }, forTargetVector));
	float dot = Vector3::Dot({ 0.0f,0.0f,-1.0f }, forTargetVector);

	//行きたい方向のQuaternionの作成
	boss->GetWorld().transform.quaternion = Quaternion::MakeRotateAxisAngleQuaternion(cross, std::acos(dot));
	if (easeT != 1.0f) {
		easeT = (std::min)(easeT + addEaseT, 1.0f);
		//ランダム生成用
		std::random_device seedGenerator;
		std::mt19937 randomEngine(seedGenerator());
		std::uniform_real_distribution<float> distribution(-ramdomvalue, ramdomvalue);
		Vector3 ramdomTranslate = { distribution(randomEngine),distribution(randomEngine) ,distribution(randomEngine) };

		boss->GetWorld().transform.translate = PrePos + ramdomTranslate;


	}
	if (easeT == 1.0f) {

	}
#ifdef _DEBUG
	ImGui::Begin("BossRocketPunch");
	ImGui::SliderFloat("ramdomvalue",&ramdomvalue,0.1f,1.0f);
	ImGui::SliderFloat("addEaseT",&addEaseT,0.01f,1.0f);
	if (ImGui::Button("ReStert")) {
		easeT = 0.0f;
	}
	ImGui::End();
#endif
}

void EAttackRocketPunch::Draw(Boss* boss)
{
	boss->Getmodels()[0]->RendererSkinDraw(boss->GetWorld(), boss->GetAnime()->GetSkinCluster());
}

std::string EAttackRocketPunch::ShowState()
{
	return "ERocketPunch";
}
