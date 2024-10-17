#include "Spider.h"
#include "Game/Object/Player/Player.h"

void Spider::Init(std::vector<Model*> models)
{
	models_ = models;
	world_.Initialize();

	//Collider
	InitCollider();

	world_.transform.translate.y = -1.0f;
#pragma region

	animation = Animation::LoadAnimationFile("resources/Monster", "Monster.gltf");

	world_.SetTransform(models_[0]);
	animation->Init();
	animation->AnimeInit(*models_[0], false);

#pragma endregion Animation

	HP_ = 4;
}

void Spider::Update()
{
	if (HP_ <= 0) {
		IsAlive = false;
	}
	//出現の時のアニメーション
	if (spawnFlame_ > kSpawnFlame_) {
		world_.transform.translate.y = 1.0f;
		isSpawn = false;
	}
	else if (isSpawn) {
		if (spawnFlame_ <= 40) {
			world_.transform.translate.y += 0.1f;
		}
		else {
			world_.transform.translate.y -= 0.1f;
		}
		spawnFlame_++;

		world_.Update();
		return;
	}

	ImGui();
	//TODO:BehaviorTreeに変更する！
	//範囲内なら攻撃
	if (ChackOnAttack() && isAttackFlag == false) {
		isAttackFlag = true;
		attackColliderFlag = true;
		//攻撃の初期化
		AttackInit();
	}

	if (isAttackFlag) {

		PlayAttackAnime();
	}
	else {
		//範囲外なら歩く
		ChasePlayer();
		LookPlayer();
	}

	DamageEffect();

	world_.Update();
}

void Spider::Draw()
{
	for (Model* model : models_) {
		model->RendererDraw(world_);
	}
}

void Spider::InitCollider()
{
	collider.Init(&world_);
	collider.SetSize({ 1.0f,1.0f,1.0f });
	collider.OnCollision = [this](ICollider* colliderB) { OnCollision(colliderB); };
	collider.SetcollitionAttribute(ColliderTag::Enemy);
	collider.SetcollisionMask(~ColliderTag::Enemy);
	//攻撃の当たり判定用座標
	attackWorld_.Initialize();
	attackWorld_.SetParent(&world_);
	attackWorld_.transform.translate = { 0.0f,0.0f,1.7f };
	attackCollider.Init(&attackWorld_);
	attackCollider.SetSize({ 1.0f,1.0f,0.7f });
	attackCollider.OnCollision = [this](ICollider* colliderB) { AttackOnCollision(colliderB); };
	attackCollider.SetcollitionAttribute(ColliderTag::EnemyAttack);
	attackCollider.SetcollisionMask(~ColliderTag::Enemy && ~ColliderTag::EnemyAttack);
	attackCollider.IsUsing = false;

}

void Spider::PlayAttackAnime()
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
	world_.Update();
}

void Spider::AttackInit()
{
	PreQua = world_.transform.quaternion;
}

bool Spider::ChackOnAttack()
{
	Vector3 PtoEdistance = player_->GetWorld().transform.translate - world_.transform.translate;
	float Distance = PtoEdistance.Length();
	if (Distance < AttackRange) {
		return true;
	}
	return false;
}

void Spider::ChasePlayer()
{
	Vector3 PtoEdistance = player_->GetWorld().transform.translate - world_.transform.translate;
	float Distance = PtoEdistance.Length();
	if (Distance < AttackRange) {
		return;
	}
	world_.transform.translate += PtoEdistance.Normalize() * 2 / 60.0f;
}

void Spider::LookPlayer()
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

void Spider::DamageEffect()
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

void Spider::AttackOnCollision(const ICollider* ICollider)
{
	if (ICollider->GetcollitionAttribute() == ColliderTag::Player) {
		attackCollider.IsUsing = false;
	}
}