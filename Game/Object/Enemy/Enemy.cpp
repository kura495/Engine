#include "Enemy.h"
#include "Game/Object/Player/Player.h"

void Enemy::PlayAttackAnime()
{
	if (isAttackFlag) {
		animationTime_ += 2.0f / 60.0f;
		if (animationTime_ > animation->duration) {
			isAttackFlag = false;
			animeInterval_ = 0.0f;
			attackCollider.IsUsing = false;
		}
		animationTime_ = std::fmod(animationTime_, animation->duration);
		//攻撃の当たり判定
		if (attackColliderFlag) {
			if (animationTime_ >= 2.0f) {
				attackCollider.IsUsing = true;
				attackColliderFlag = false;
			}
		}
		//アニメーション情報を取得
		NodeAnimation& rootNodeAnimation = animation->nodeAnimations[models_[0]->GetModelData().rootNode.name];
		Vector3 translate = Animation::CalculateValue(rootNodeAnimation.translate.keyFrames, animationTime_);
		Quaternion rotation = Animation::CalculateValue(rootNodeAnimation.rotate.keyFrames, animationTime_);

		Vector3 scale = Animation::CalculateValue(rootNodeAnimation.scale.keyFrames, animationTime_);
		world_.transform.translate += translate;
		world_.transform.quaternion = PreQua * rotation;
		world_.transform.scale = scale;
	}
	world_.UpdateMatrix();
}

void Enemy::ImGui()
{
#ifdef USE_IMGUI
	ImGui::Begin("Enemy");
	ImGui::DragFloat3("Scale", &world_.transform.scale.x);
	ImGui::DragFloat4("Rotate", &world_.transform.quaternion.x);
	ImGui::DragFloat3("Translate", &world_.transform.translate.x);
	if (ImGui::Button("CollisionOn")) {
		collider.IsUsing = true;
	}
	if (ImGui::Button("CollisionOff")) {
		collider.IsUsing = false;
	}
	ImGui::Text("HP : %d", HP_);
	ImGui::End();
#endif
}

void Enemy::OnCollision(const ICollider* ICollider)
{
	if (ICollider->GetcollitionAttribute() == ColliderTag::Weapon) {
		HP_ -= 1;
		isDamege = true;
		damegeInterval = 0;

	}
	if (ICollider->GetcollitionAttribute() == ColliderTag::Player) {
	}
}

void Enemy::AttackOnCollision(const ICollider* ICollider)
{
	if (ICollider->GetcollitionAttribute() == ColliderTag::Player) {
		attackCollider.IsUsing = false;
	}
}

void Enemy::AttackInit()
{
	PreQua = world_.transform.quaternion;
}

bool Enemy::ChackOnAttack()
{
	Vector3 PtoEdistance = player_->GetWorld().transform.translate - world_.transform.translate;
	float Distance = PtoEdistance.Length();
	if (Distance < AttackRange) {
		return true;
	}
	return false;
}

void Enemy::ChasePlayer()
{
	Vector3 PtoEdistance = player_->GetWorld().transform.translate - world_.transform.translate;
	float Distance = PtoEdistance.Length();
	if (Distance < AttackRange) {
		return;
	}
	world_.transform.translate += PtoEdistance.Normalize() * 2 / 60.0f;
}

void Enemy::LookPlayer()
{
	// 自機に向かうベクトル
	Vector3 targetVel = (player_->GetWorld().transform.translate - world_.transform.translate).Normalize();
	targetVel.y = 0;
	//プレイヤーの現在の向き
	targetVel = targetVel.Normalize();

	Vector3 cross = Vector3::Cross({ 0.0f,0.0f,1.0f }, targetVel).Normalize();
	float dot = Vector3::Dot({ 0.0f,0.0f,1.0f }, targetVel);

	//行きたい方向のQuaternionの作成
	world_.transform.quaternion = Quaternion::MakeRotateAxisAngleQuaternion(cross, std::acos(dot));
}

void Enemy::DamageEffect()
{
	//TODO：モデルをポインタで受け取っているため、color_を変えるとすべての敵キャラの色が変わる
	if (isDamege) {
		damegeInterval++;
		if (damegeInterval % 10 == 0) {

			models_[0]->color_ = { 1.0f,0.5f,0.5f,1.0f };
		}
		else {
			models_[0]->color_ = { 1.0f,1.0f,1.0f,1.0f };
		}
	}
	if (damegeInterval > kDamegeInterval) {
		models_[0]->color_ = { 1.0f,1.0f,1.0f,1.0f };
		isDamege = false;
	}
}
