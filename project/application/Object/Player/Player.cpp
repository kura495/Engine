#include "Player.h"

void Player::Init(std::vector<Model*> models)
{
	name = "Player";

	input = Input::GetInstance();

	models_ = models;
	//ワールド設定
	world_.Initialize();
	world_.transform.translate.z = -3.0f;
	world_.Update();
	//ライトの設定
	models_[0]->GetMaterial()->enableLighting = Lighting::NotDo;
	//コライダー設定
	ColliderInit();
	AttackColliderInit();
	//アニメーション設定
#pragma region
	walkanimation = new Animation();
	walkanimation = Animation::LoadAnimationFile("project/resources/Player", "player_walk.gltf");
	walkanimation->Init();
	walkanimation->AnimeInit(*models_[0], true);

	deadAnimation = new Animation();
	deadAnimation = Animation::LoadAnimationFile("project/resources/Player", "player_dead.gltf");
	deadAnimation->Init();
	deadAnimation->AnimeInit(*models_[0], true);
	
	attackAnimation = new Animation();
	attackAnimation = Animation::LoadAnimationFile("project/resources/Player", "player_attack.gltf");
	attackAnimation->Init();
	attackAnimation->AnimeInit(*models_[0], true);
#pragma endregion Anime
	//パーティクル設定
#pragma region
	deadParticle_ = new ParticleSystem();
	deadParticle_->Init("project/resources/circle2.dds");
	deadParticle_->UpdateFunc = [this](Particle& particle) {return UpdatedeadParticle(particle); };

	deadParticleEmitter.count = 5;
	deadParticleEmitter.frequency = 0.5f;
	deadParticleEmitter.particleRadius = {0.5f,0.5f,1.0f};
	deadParticleEmitter.color = { 0.0f,0.0f,0.0f };
	deadParticleEmitter.speed = { 1.0f,1.0f,1.0f };

	attackHitParticle_ = new ParticleSystem();
	attackHitParticle_->Init("project/resources/circle2.dds");
	attackHitParticle_->UpdateFunc = [this](Particle& particle) {return UpdateAttackHitParticle(particle); };
	AttackHitParticleEmitter.count = 20;
	AttackHitParticleEmitter.frequency = 0.1f;
	AttackHitParticleEmitter.particleRadius = {0.5f,0.5f,1.0f};
	AttackHitParticleEmitter.color = { 1.0f,0.5f,0.5f };
	AttackHitParticleEmitter.speed = { 5.0f,3.5f,5.0f };

	attackHitBombParticle_ = new ParticleSystem();
	attackHitBombParticle_->Init("project/resources/circle2.dds");
	attackHitBombParticle_->UpdateFunc = [this](Particle& particle) {return UpdateAttackHitBombParticle(particle); };
	AttackHitBombParticleEmitter.count = 5;
	AttackHitBombParticleEmitter.frequency = 0.1f;
	AttackHitBombParticleEmitter.particleRadius = {0.1f,0.1f,1.0f};
	AttackHitBombParticleEmitter.color = { 0.5f,0.5f,1.0f };
	AttackHitBombParticleEmitter.speed = { 5.0f,3.5f,5.0f };
#pragma endregion パーティクル
	//音声
	SEattack = Audio::LoadAudioMP3("project/resources/sound/Player/attack.mp3",false);
	SEHitattack = Audio::LoadAudioMP3("project/resources/sound/Player/Hitattack.mp3",false);

	ChangeState<PRoot>();
}
void Player::TitleUpdate()
{
	//待機モーション再生
	walkanimation->PlayAnimation();
}
void Player::Update()
{
	gravity += kgravity;
	//パッドの状態をゲット
	input->GetJoystickState(joyState);
	
	if (isDeadFlag == false) {
		//ダメージを受けた時、落っこちた時の処理
		if (isDamege && HP_ >= 1 || world_.transform.translate.y <= -10.0f) {
			isDeadFlag = true;
			ChangeState<PDead>();
			isDamege = false;
			HP_ -= 1;
		}
	}
	//ステートのアップデート
	state_->Update(this);
	//パーティクルアップデート
	attackHitParticle_->Update();
	attackHitBombParticle_->Update();
	deadParticle_->Update();

#ifdef USE_IMGUI
	ImGui();
#endif
	world_.Update();

	isOnFloorFlag = false;
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
	//描画
	state_->Draw(this);
	//パーティクル描画
	attackHitParticle_->RendererDraw();
	attackHitBombParticle_->RendererDraw();
}
#pragma region

//kRoot
void Player::RootInit()
{
	colliderPlayer.IsUsing = true;
	animationTime_ = 0.0f;
	walkanimation->Reset();
	isMovedFlag = false;

}
void Player::RootUpdate()
{
	Move();
	//動いていたら
	if (isMovedFlag) {
		//動いていたら歩きモーションを再生
		walkanimation->PlayAnimation();

		animationTime_ += 1.0f / 60.0f;
		if (animationTime_ > deadAnimation->duration) {
			animationTime_ = 0.0f;
			isMovedFlag = false;
		}
	}
	//重力
	world_.transform.translate.y -= gravity;
	//ボタンを押したら攻撃
	if (input->GetPadPrecede(XINPUT_GAMEPAD_X, 20)) {
		ChangeState<PAttack>();
	}
	//ボタンを押したらジャンプ
	else if (input->IsTriggerPad(XINPUT_GAMEPAD_A)) {
		ChangeState<PJump>();
	}
}
void Player::RootDraw()
{
	models_[0]->RendererSkinDraw(world_, walkanimation->GetSkinCluster());
}
//kAttack
void Player::AttackInit()
{
	colliderAttack.IsUsing = true;
	animationTime_ = 0.0f;
	attackPosture = world_.transform.quaternion;
	isMovedFlag = false;
	Audio::Play(SEattack, 0.5f);
	Audio::Stop(SEHitattack, true, false);
}
void Player::AttackUpdate()
{
	//攻撃中も移動できるように
	Move();
	//攻撃中は一定の方向を向くように固定
	world_.transform.quaternion = attackPosture;
	//アニメーション再生
	attackAnimation->PlayAnimation();
	animationTime_ += 1.0f / 60.0f;
	//重力
	world_.transform.translate.y -= gravity;

	if (animationTime_ > attackAnimation->duration) {
		animationTime_ = 0.0f;
		colliderAttack.IsUsing = false;
		//kRootに戻す
		ChangeState<PRoot>();
		attackAnimation->Reset();
		Audio::Stop(SEattack, true, false);
	}

}
void Player::AttackDraw()
{
	models_[0]->RendererSkinDraw(world_, attackAnimation->GetSkinCluster());
}
//kJump
void Player::JumpInit() {
	jumpForce = kJumpForce;
}
void Player::JumpUpdate() {
	//移動関数
	Move();
	//ジャンプの処理
	world_.transform.translate.y += jumpForce;
	jumpForce -= kJumpSubValue;
}
void Player::JumpDraw()
{
	models_[0]->RendererSkinDraw(world_, walkanimation->GetSkinCluster());
}
//kDead
void Player::DeadInit()
{
	deadParticleEmitter.world_.transform.translate = world_.transform.translate;
	deadParticleEmitter.world_.transform.translate.y += 1.0f;
	deadParticleEmitter.frequencyTime = deadParticleEmitter.frequency;
	animationTime_ = 1.0f / 60.0f;
}
void Player::DeadUpdate()
{
	isOnFloorFlag = true;
	//死亡アニメーション更新

	animationTime_ += 1.0f / 60.0f;
	deadAnimation->PlayAnimation(false);
	if (animationTime_ >= deadAnimation->duration) {
		isDamege = false;
		animationTime_ = 0.0f;
		isDeadModelDraw = false;
		isDead = true;
	}
	if (isDeadModelDraw == false) {
		//パーティクル生成
		ParticleSystem::ParticleSpawn(*deadParticle_, deadParticleEmitter);
		deadParticle_->Update();
	}
}
void Player::DeadDraw()
{
	if (isDeadModelDraw) {
		models_[0]->RendererSkinDraw(world_, deadAnimation->GetSkinCluster());
	}
	else {
		deadParticle_->RendererDraw();
	}
}
#pragma endregion State
#pragma region
void Player::ColliderInit()
{
	colliderPlayer.Init(&world_);
	colliderPlayer.SetSize({ 0.5f,0.7f,0.5f });
	colliderPlayer.SetOffset({ 0.0f,0.7f,0.0f });
	colliderPlayer.OnCollision = [this](ICollider& collider) { OnCollision(collider); };
	colliderPlayer.SetcollitionAttribute(ColliderTag::Player);
	colliderPlayer.SetcollisionMask(~ColliderTag::Player & ~ColliderTag::Weapon);
}
void Player::OnCollision(const ICollider& ICollider)
{
	if (ICollider.GetcollitionAttribute() == ColliderTag::EnemyAttack) {
		isDamege = true;
		return;
	}
	if (ICollider.GetcollitionAttribute() == ColliderTag::EnemyBall) {
		isDamege = true;
		return;
	}
	if (ICollider.GetcollitionAttribute() == ColliderTag::Enemy) {
		world_.transform.translate -= move;
		world_.Update();
		return;
	}
	if (ICollider.GetcollitionAttribute() == ColliderTag::Weapon) {
		return;
	}
	if (ICollider.GetcollitionAttribute() == ColliderTag::Floor) {
		if (state_->GetStateType() == PlayerState::kJump) {
			if (world_.transform.translate.y <= ICollider.GetCenter().y && jumpForce <= 0) {
				world_.transform.translate.y = ICollider.GetCenter().y;
				world_.Update();
				gravity = 0.0f;
				ChangeState<PRoot>();
			}
		}
		else {
			if (isOnFloorFlag) {
				return;
			}
			world_.transform.translate.y += gravity;
			isOnFloorFlag = true;
			gravity = kgravity;
			world_.Update();
		}
		//重力分
		return;
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
	colliderAttack.SetcollisionMask(~ColliderTag::Player & ~ColliderTag::Weapon & ~ColliderTag::Floor);

	colliderAttack.IsUsing = false;
}
void Player::AttackOnCollision(const ICollider& collider)
{
	if (collider.GetcollitionAttribute() == ColliderTag::EnemyCore) {
		colliderAttack.IsUsing = false;
		//パーティクル用のベクトル
		attackVector = TransformNormal({0.0f,0.0f,1.0f},Matrix4x4 (MakeRotateMatrix(world_.transform.quaternion)));
		attackVector.Normalize();
		attackVector *= -1;
		//ランダム生成用
		std::random_device seedGenerator;
		std::mt19937 randomEngine(seedGenerator());
		//パーティクル生成
		AttackHitParticleEmitter.world_.transform.translate = attackColliderWorld_.transform.translate + world_.transform.translate;
		AttackHitParticleEmitter.world_.transform.translate.y += 1.0f;
		attackHitParticle_->SpawnParticle(AttackHitParticleEmitter, randomEngine);
		//音関連
		Audio::Stop(SEattack, true,false);
		Audio::Play(SEHitattack, 1.0f);

	}
	if (collider.GetcollitionAttribute() == ColliderTag::EnemyBall) {
		colliderAttack.IsUsing = false;
		//パーティクル用のベクトル
		attackVector = TransformNormal({ 0.0f,0.0f,1.0f }, Matrix4x4(MakeRotateMatrix(world_.transform.quaternion)));
		attackVector.Normalize();
		attackVector *= -1;
		//ランダム生成用
		std::random_device seedGenerator;
		std::mt19937 randomEngine(seedGenerator());
		//パーティクル生成
		AttackHitBombParticleEmitter.color = { 0.5f,0.5f,1.0f };
		AttackHitBombParticleEmitter.world_.transform.translate = attackColliderWorld_.transform.translate + world_.transform.translate;
		AttackHitBombParticleEmitter.world_.transform.translate.y += 1.0f;
		attackHitBombParticle_->SpawnParticle(AttackHitBombParticleEmitter, randomEngine);
	}
	if (collider.GetcollitionAttribute() == ColliderTag::FakeEnemyBall) {
		colliderAttack.IsUsing = false;
		//パーティクル用のベクトル
		attackVector = TransformNormal({ 0.0f,0.0f,1.0f }, Matrix4x4(MakeRotateMatrix(world_.transform.quaternion)));
		attackVector.Normalize();
		attackVector *= -1;
		//ランダム生成用
		std::random_device seedGenerator;
		std::mt19937 randomEngine(seedGenerator());
		//パーティクル生成
		AttackHitBombParticleEmitter.color = { 1.0f,1.0f,1.0f };
		AttackHitBombParticleEmitter.world_.transform.translate = attackColliderWorld_.transform.translate + world_.transform.translate;
		AttackHitBombParticleEmitter.world_.transform.translate.y += 1.0f;
		attackHitBombParticle_->SpawnParticle(AttackHitBombParticleEmitter, randomEngine);
	}
}
#pragma endregion Collider
void Player::UpdatedeadParticle(Particle& particle)
{
	//速度を1/60にする
	Vector3 velcity = particle.velocity * kDeltaTime;
	particle.transform.translate += velcity * deadParticleEmitter.speed;
	//エミッターがパーティクルの半径を決める
	particle.transform.scale = deadParticleEmitter.particleRadius;
	Vector3 translate = particle.transform.translate;
	float alpha = 1.0f - (particle.currentTime / particle.lifeTime);
	//色をセット
	particle.color.w = alpha;
	particle.color.x = deadParticleEmitter.color.x;
	particle.color.y = deadParticleEmitter.color.y;
	particle.color.z = deadParticleEmitter.color.z;
	particle.currentTime += kDeltaTime;
	particle.matWorld = MakeAffineMatrix(particle.transform.scale, Vector3{ 0.0f,0.0f,0.0f }, translate);
}
void Player::UpdateAttackHitParticle(Particle& particle)
{
	//速度を1/60にする
	Vector3 velcity = particle.velocity + (AttackHitBombParticleEmitter.speed * attackVector);
	particle.transform.translate += velcity * kDeltaTime;
	//エミッターがパーティクルの半径を決める
	particle.transform.scale = AttackHitParticleEmitter.particleRadius;
	Vector3 translate = particle.transform.translate;
	float alpha = 1.0f - (particle.currentTime / particle.lifeTime);
	//色をセット
	particle.color.w = alpha;
	particle.color.x = AttackHitParticleEmitter.color.x;
	particle.color.y = AttackHitParticleEmitter.color.y;
	particle.color.z = AttackHitParticleEmitter.color.z;
	particle.currentTime += kDeltaTime;
	particle.matWorld = MakeAffineMatrix(particle.transform.scale, Vector3{ 0.0f,0.0f,0.0f }, translate);
}
void Player::UpdateAttackHitBombParticle(Particle& particle)
{
	//速度を1/60にする
	Vector3 velcity = particle.velocity + (AttackHitBombParticleEmitter.speed * attackVector);
	particle.transform.translate += velcity * kDeltaTime;
	//エミッターがパーティクルの半径を決める
	particle.transform.scale = AttackHitBombParticleEmitter.particleRadius;
	Vector3 translate = particle.transform.translate;
	float alpha = 1.0f - (particle.currentTime / particle.lifeTime);
	//色をセット
	particle.color.w = alpha;
	particle.color.x = AttackHitBombParticleEmitter.color.x;
	particle.color.y = AttackHitBombParticleEmitter.color.y;
	particle.color.z = AttackHitBombParticleEmitter.color.z;
	particle.currentTime += kDeltaTime;
	particle.matWorld = MakeAffineMatrix(particle.transform.scale, Vector3{ 0.0f,0.0f,0.0f }, translate);
}
void Player::ImGui()
{
	ImGui::Begin("Player");
	ImGui::DragFloat3("Scale", &world_.transform.scale.x);
	ImGui::DragFloat4("Rotate", &world_.transform.quaternion.x);
	ImGui::DragFloat3("Translate", &world_.transform.translate.x, 0.1f);
	ImGui::Text("%d", HP_);
	ImGui::Text("%f", jumpForce);
	ImGui::Text(state_->ShowState().c_str());
	ImGui::End();
}
void Player::Move()
{
	//加算量を0に戻す
	move = {0.0f,0.0f,0.0f };
	//移動量
	if (joyState.Gamepad.sThumbLX != 0 && joyState.Gamepad.sThumbLY != 0) {

#pragma region
		//スティックから移動量を計算
		move = {
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