#include "Player.h"

void Player::Init(std::vector<Model*> models)
{
	name = "Player";

	input = Input::GetInstance();

	models_ = models;
	world_.Initialize();
	world_.transform.translate.z = -3.0f;

	world_.Update();

	models_[0]->GetMaterial()->enableLighting = Lighting::NotDo;

	ColliderInit();
	AttackColliderInit();

#pragma region
	walkanimation = new Animation();
	walkanimation = Animation::LoadAnimationFile("resources/Player", "player_walk.gltf");
	walkanimation->Init();
	walkanimation->AnimeInit(*models_[0], true);

	deadAnimation = new Animation();
	deadAnimation = Animation::LoadAnimationFile("resources/Player", "player_dead.gltf");
	deadAnimation->Init();
	deadAnimation->AnimeInit(*models_[0], true);
	
	attackAnimation = new Animation();
	attackAnimation = Animation::LoadAnimationFile("resources/Player", "player_attack.gltf");
	attackAnimation->Init();
	attackAnimation->AnimeInit(*models_[0], true);
#pragma endregion Anime

#pragma region
	particle_ = new ParticleSystem();
	particle_->Initalize("resources/circle2.png");
	particle_->UpdateParticle = [this](Particle& particle) {return UpdateParticle(particle); };

	deadParticleEmitter.count = 5;
	deadParticleEmitter.frequency = 0.1f;
	deadParticleEmitter.particleRadius = {0.5f,0.5f,1.0f};
	deadParticleEmitter.color = { 0.0f,0.0f,0.0f };
	deadParticleEmitter.speed = { 2.0f,2.0f,2.0f };
#pragma endregion パーティクル
}

void Player::TitleUpdate()
{
	walkanimation->PlayAnimation();
}

void Player::Update()
{

	//パッドの状態をゲット
	input->GetJoystickState(joyState);

	if (isDamege && HP_ >= 1) {
		behaviorRequest_ = Behavior::kDead;
		isDamege = false;
		HP_ -= 1;
	}

	BehaviorUpdate();

	particle_->Update();
	deadParticleEmitter.world_.transform.translate = world_.transform.translate;
#ifdef USE_IMGUI
	ImGui();
#endif
	//TODO　着地面の高さを求めて、カメラの高さを決めると、アストロボット的なジャンプが実装できそう
	//		ジャンプ先が高い位置にあると、カメラが上に移動する
	if (behavior_ != Behavior::kJump) {
		//FollowCamera::workInter.interParameter_.y = (std::min)(FollowCamera::workInter.interParameter_.y + 0.1f, 1.0f);

	}

	world_.Update();

	//前フレームのゲームパッドの状態を保存
	joyStatePre = joyState;
}

void Player::TitleDraw()
{
	models_[0]->RendererDraw(world_);
}

void Player::Draw()
{
#ifdef _DEBUG
	//walkanimation->DebugDraw(world_);
#endif
	

	switch (behavior_)
	{
	case Behavior::kRoot:
	default:
		models_[0]->RendererSkinDraw(world_, walkanimation->GetSkinCluster());
		//models_[0]->RendererDraw(world_);
		break;
	case Behavior::kAttack:
		models_[0]->RendererSkinDraw(world_, attackAnimation->GetSkinCluster());
		break;
	case Behavior::kJump:
		models_[0]->RendererSkinDraw(world_, walkanimation->GetSkinCluster());
		break;
	case Behavior::kDead:
		if (isDeadModelDraw) {
			models_[0]->RendererSkinDraw(world_, deadAnimation->GetSkinCluster());
		}
		else {
			particle_->RendererDraw();
		}
		break;
	}

}

#pragma region
void Player::BehaviorUpdate()
{
	//初期化
	if (behaviorRequest_) {
		//ふるまいの変更
		behavior_ = behaviorRequest_.value();
		//各ふるまいごとに初期化
		switch (behavior_)
		{
		case Behavior::kRoot:
		default:
			RootInit();
			break;
		case Behavior::kAttack:
			AttackInit();
			break;
		case Behavior::kJump:
			JumpInit();
			break;
		case Behavior::kDead:
			DeadInit();
			break;
		}

		behaviorRequest_ = std::nullopt;
	}
	//更新
	switch (behavior_)
	{
	case Behavior::kRoot:
	default:
		RootUpdate();
		break;
	case Behavior::kAttack:
		AttackUpdate();
		break;
	case Behavior::kJump:
		JumpUpdate();
		break;
	case Behavior::kDead:
		DeadUpdate();
		break;
	}
}
//kRoot
void Player::RootInit()
{
	colliderPlayer.IsUsing = true;
	animationTime_ = 0.0f;

}
void Player::RootUpdate()
{
	Move();
	//動いていたら歩きモーションを再生
	if (isMovedFlag) {
		animationTime_ += 1.0f / 60.0f;
		if (animationTime_ > deadAnimation->duration) {
			animationTime_ = 0.0f;
			isMovedFlag = false;
		}
		walkanimation->PlayAnimation();
	}

	//攻撃
	if (input->GetPadPrecede(XINPUT_GAMEPAD_X, 20)) {
		behaviorRequest_ = Behavior::kAttack;
	}
	//ジャンプ
	else if (input->IsTriggerPad(XINPUT_GAMEPAD_A)) {
		behaviorRequest_ = Behavior::kJump;
	}
}
//kAttack
void Player::AttackInit()
{
	colliderAttack.IsUsing = true;
	animationTime_ = 0.0f;
	attackPosture = world_.transform.quaternion;
	isMovedFlag = false;

}
void Player::AttackUpdate()
{
	//攻撃中も移動できるように
	Move();
	//攻撃中は一定の方向を向くように固定
	world_.transform.quaternion = attackPosture;

	animationTime_ += 1.0f / 60.0f;

	if (animationTime_ > attackAnimation->duration) {
		animationTime_ = 0.0f;
		colliderAttack.IsUsing = false;
		//kRootに戻す
		behaviorRequest_ = Behavior::kRoot;
	}
	//アニメーション再生
	attackAnimation->PlayAnimation();
}
//kJump
void Player::JumpInit() {
	jumpForce = kJumpForce;
	//FollowCamera::workInter.interParameter_.y = 0.0f;
}
void Player::JumpUpdate() {

	Move();

	world_.transform.translate.y += jumpForce;
	jumpForce -= kJumpSubValue;

	if (world_.transform.translate.y <= 0) {
		world_.transform.translate.y = 0;
		behaviorRequest_ = Behavior::kRoot;
	}
}
//kDead
void Player::DeadInit()
{
	deadParticleEmitter.world_.transform.translate = world_.transform.translate;
	deadParticleEmitter.world_.transform.translate.y += 1.0f;
	animationTime_ = 0.0f;
}
void Player::DeadUpdate()
{
	//パーティクル
	deadParticleEmitter.frequencyTime += kDeltaTime;
	if (deadParticleEmitter.frequency <= deadParticleEmitter.frequencyTime) {
		//ランダム生成用
		std::random_device seedGenerator;
		std::mt19937 randomEngine(seedGenerator());

		particle_->SpawnParticle(deadParticleEmitter, randomEngine);

		deadParticleEmitter.frequencyTime -= deadParticleEmitter.frequency;
	}

	deadAnimation->PlayAnimation();

	animationTime_ += 1.0f / 60.0f;
	if (animationTime_ > deadAnimation->duration) {
		isDamege = false;
		animationTime_ = 0.0f;
		if (HP_ <= 0) {
			isDeadModelDraw = false;
			isDead = true;
		}
	}
	if (isDeadModelDraw == false) {
		particle_->Update();
	}
}
#pragma endregion BeheviorTree
#pragma region
void Player::ColliderInit()
{
	colliderPlayer.Init(&world_);
	colliderPlayer.SetSize({ 0.5f,1.0f,0.5f });
	colliderPlayer.SetOffset({ 0.0f,0.5f,0.0f });
	colliderPlayer.OnCollision = [this](ICollider& collider) { OnCollision(collider); };
	colliderPlayer.SetcollitionAttribute(ColliderTag::Player);
	colliderPlayer.SetcollisionMask(~ColliderTag::Player && ~ColliderTag::Weapon);
}
void Player::OnCollision(const ICollider& ICollider)
{

	if (ICollider.GetcollitionAttribute() == ColliderTag::EnemyAttack) {
		isDamege = true;
		ImGui::Begin("Player");
		ImGui::Text("Hit");
		ImGui::End();
		//playerMoveValue = true;
	}
	if (ICollider.GetcollitionAttribute() == ColliderTag::EnemyBomb) {
		isDamege = true;
		ImGui::Begin("Player");
		ImGui::Text("Hit");
		ImGui::End();
	}

	return;
}
void Player::AttackColliderInit()
{
	attackColliderWorld_.SetParent(&world_);
	colliderAttack.Init(&attackColliderWorld_);
	colliderAttack.SetSize({ 1.0f,1.0f,1.0f });
	colliderAttack.SetOffset({ 0.0f,0.5f,1.0f });
	colliderAttack.OnCollision = [this](ICollider& collider) { AttackOnCollision(collider); };
	colliderAttack.SetcollitionAttribute(ColliderTag::Weapon);
	colliderAttack.SetcollisionMask(~ColliderTag::Player && ~ColliderTag::Weapon);

	colliderAttack.IsUsing = false;
}
void Player::AttackOnCollision(const ICollider& collider)
{
	if (collider.GetcollitionAttribute() == ColliderTag::Enemy) {
		colliderAttack.IsUsing = false;
	}
	if (collider.GetcollitionAttribute() == ColliderTag::EnemyBomb) {
		colliderAttack.IsUsing = false;
	}
}
#pragma endregion Collider
void Player::UpdateParticle(Particle& particle)
{

	Vector3 velcity = particle.velocity * kDeltaTime;
	particle.transform.translate += velcity * deadParticleEmitter.speed;
	//エミッターがパーティクルの半径を決める
	particle.transform.scale = deadParticleEmitter.particleRadius;
	Vector3 translate = particle.transform.translate;
	float alpha = 1.0f - (particle.currentTime / particle.lifeTime);
	particle.color.w = alpha;
	particle.color.x = deadParticleEmitter.color.x;
	particle.color.y = deadParticleEmitter.color.y;
	particle.color.z = deadParticleEmitter.color.z;
	particle.currentTime += kDeltaTime;
	particle.matWorld = MakeAffineMatrix(particle.transform.scale, Vector3{ 0.0f,0.0f,0.0f }, translate);
}
void Player::ImGui()
{
	ImGui::Begin("Player");
	ImGui::DragFloat3("Scale", &world_.transform.scale.x);
	ImGui::DragFloat4("Rotate", &world_.transform.quaternion.x);
	ImGui::DragFloat3("Translate", &world_.transform.translate.x, 0.1f);
	if (ImGui::Button("Reset")) {
		world_.transform.translate = { 0.0f,2.0f,0.0f };
		world_.transform.quaternion = Quaternion::IdentityQuaternion();
	}
	if (ImGui::Button("CollisionOn")) {
		colliderPlayer.IsUsing = true;
	}
	if (ImGui::Button("CollisionOff")) {
		colliderPlayer.IsUsing = false;
	}
	ImGui::Text("%d", HP_);
	ImGui::End();
}

void Player::Move()
{
	//移動量
	if (joyState.Gamepad.sThumbLX != 0 && joyState.Gamepad.sThumbLY != 0) {

#pragma region
		Vector3 move = {
		(float)joyState.Gamepad.sThumbLX / SHRT_MAX, 0.0f,
		(float)joyState.Gamepad.sThumbLY / SHRT_MAX };
		//正規化をして斜めの移動量を正しくする
		move = move.Normalize();
		move.x = move.x * kMoveSpeed_;
		move.y = move.y * kMoveSpeed_;
		move.z = move.z * kMoveSpeed_;
		//カメラの正面方向に移動するようにする
		//回転行列を作る
		Matrix4x4 rotateMatrix = MakeRotateMatrix(Renderer::viewProjection.rotation_);
		//移動ベクトルをカメラの角度だけ回転
		move = TransformNormal(move, rotateMatrix);
		move.y = 0.0f;
		//移動
		world_.transform.translate = world_.transform.translate + move;

#pragma endregion 移動
#pragma region
		//移動ベクトルをカメラの角度だけ回転
		//追従対象からロックオン対象へのベクトル
		Vector3 sub = move;

		//プレイヤーの現在の向き
		sub = sub.Normalize();

		Vector3 cross = Vector3::Normalize(Vector3::Cross({ 0.0f,0.0f,1.0f }, sub));
		float dot = Vector3::Dot({ 0.0f,0.0f,1.0f }, sub);

		//行きたい方向のQuaternionの作成
		world_.transform.quaternion = Quaternion::MakeRotateAxisAngleQuaternion(cross, std::acos(dot));

#pragma endregion プレイヤーの回転

		isMovedFlag = true;

		return;
	}


}