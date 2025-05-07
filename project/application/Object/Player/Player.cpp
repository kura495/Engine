#include "Player.h"
#include "Scenes/State/Play/GamePlayPhase/PlayPhase/PlayPhase.h"
#include "Boss.h"
void Player::Init(std::vector<Model*> models)
{
	name = "Player";

	input = Input::GetInstance();

	models_ = models;
	//ワールド設定
	world_.Init();
	world_.transform.translate.z = -3.0f;
	world_.Update();
	//ライトの設定
	models_[PlayerModel::MainBody]->GetMaterial()->enableLighting = Lighting::NotDo;
	//コライダー設定
	ColliderInit();
	AttackColliderInit();
	//パーティクル設定
#pragma region

	attackHitParticle_ = std::make_unique<ParticleSystem>();
	attackHitParticle_->Init("project/resources/circle2.dds");
	attackHitParticle_->UpdateFunc = [this](Particle& particle) {return UpdateAttackHitParticle(particle); };
	AttackHitParticleEmitter.count = 20;
	AttackHitParticleEmitter.frequency = 0.1f;
	AttackHitParticleEmitter.particleRadius = {0.5f,0.5f,1.0f};
	AttackHitParticleEmitter.color = { 1.0f,0.5f,0.5f };
	AttackHitParticleEmitter.speed = { 5.0f,3.5f,5.0f };

	attackHitBombParticle_ = std::make_unique<ParticleSystem>();
	attackHitBombParticle_->Init("project/resources/circle2.dds");
	attackHitBombParticle_->UpdateFunc = [this](Particle& particle) {return UpdateAttackHitBombParticle(particle); };
	AttackHitBombParticleEmitter.count = 5;
	AttackHitBombParticleEmitter.frequency = 0.1f;
	AttackHitBombParticleEmitter.particleRadius = {0.1f,0.1f,1.0f};
	AttackHitBombParticleEmitter.color = { 0.5f,0.5f,1.0f };
	AttackHitBombParticleEmitter.speed = { 5.0f,3.5f,5.0f };
#pragma endregion パーティクル
	//音声
	SEattack = Audio::LoadAudioMP3("project/resources/sound/Player/attack.mp3", false);
	SEHitattack = Audio::LoadAudioMP3("project/resources/sound/Player/Hitattack.mp3", false);
	ChangeState<PRoot>();
}
void Player::Update()
{
	//パッドの状態をゲット
	input->GetJoystickState(joyState);
	if (isDyingFlag == false) {
		//ダメージを受けた時、落っこちた時の処理
		if (isDamege && HP_ >= 1 || world_.transform.translate.y <= -10.0f) {
			isDyingFlag = true;
			ChangeState<PDead>();
			isDamege = false;
			HP_ -= 1;
			colliders_[ColliderType::pCollider].IsUsing = false;
		}
	}
	//ステートのアップデート
	state_->Update(this);
	//パーティクルアップデート
	attackHitParticle_->Update();
	attackHitBombParticle_->Update();
	//重力を加える
	if (state_->GetStateType() != PlayerState::Dead && state_->GetStateType() != PlayerState::Jump) {
		world_.transform.translate.y -= gravity;
	}
	//地面にいないなら落ちるスピードが加速する
	if (isOnFloorFlag == false) {
		gravity = std::min(gravity + kgravity,0.98f);
	}
	isOnFloorFlag = false;
#ifdef USE_IMGUI
	ImGui();
#endif
	world_.Update();
}
void Player::Draw()
{
	//描画
	state_->Draw(this);
	//パーティクル描画
	attackHitParticle_->RendererDraw();
	attackHitBombParticle_->RendererDraw();
}
void Player::SetColliderUse(int number, bool flag)
{
	colliders_[number].IsUsing = flag;
}
void Player::SetColliderAttribute(int number, uint32_t collisionAttribute)
{
	colliders_[number].SetcollitionAttribute(collisionAttribute);
}
#pragma region
void Player::ColliderInit()
{
	colliders_[ColliderType::pCollider].Init(&world_);
	colliders_[ColliderType::pCollider].SetSize(colliderSize);
	colliders_[ColliderType::pCollider].SetOffset(colliderOffset);
	colliders_[ColliderType::pCollider].OnCollision = [this](ICollider& collider) { OnCollision(collider); };
	colliders_[ColliderType::pCollider].SetcollitionAttribute(Collider::Tag::Player);
	colliders_[ColliderType::pCollider].SetcollisionMask(~Collider::Tag::Player & ~Collider::Tag::Weapon);
	colliders_[ColliderType::pCollider].colliderName = "Player";
}
void Player::OnCollision(const ICollider& ICollider)
{
	if (ICollider.GetcollitionAttribute() == Collider::Tag::EnemyAttackSlam) {
		isDamege = true;
		causeOfDeath_ = CauseOfDeath::Slam;
	}
	if (ICollider.GetcollitionAttribute() == Collider::Tag::EnemyBall) {
		isDamege = true;
		causeOfDeath_ = CauseOfDeath::Normal;
	}
	if (ICollider.GetcollitionAttribute() == Collider::Tag::EnemyAttack) {
		isDamege = true;
		causeOfDeath_ = CauseOfDeath::Normal;
	}
	if (ICollider.GetcollitionAttribute() == Collider::Tag::Enemy) {

		world_.transform.translate -= move;
		world_.Update();
	}
	if (ICollider.GetcollitionAttribute() == Collider::Tag::Floor) {
		world_.transform.translate.y = ICollider.GetCenter().y;
		world_.Update();
		gravity = kgravity;
		isOnFloorFlag = true;
	}
#ifdef _DEBUG
	ImGui::Begin("pushForceLog");
	for (Vector3 aaaaa : pushForce) {
		ImGui::DragFloat3("colliderA", &aaaaa.x);
	}
	ImGui::End();
#endif
	state_->OnCollision(this,ICollider);
	return;
}
void Player::AttackColliderInit()
{
	attackColliderWorld_.SetParent(&world_);
	colliders_[ColliderType::Attack].Init(&attackColliderWorld_);
	colliders_[ColliderType::Attack].SetSize({ 1.0f,1.0f,1.0f });
	colliders_[ColliderType::Attack].SetOffset(attackColliderOffset);
	colliders_[ColliderType::Attack].OnCollision = [this](ICollider& collider) { AttackOnCollision(collider); };
	colliders_[ColliderType::Attack].SetcollitionAttribute(Collider::Tag::Weapon);
	colliders_[ColliderType::Attack].SetcollisionMask(~Collider::Tag::Player & ~Collider::Tag::Weapon & ~Collider::Tag::Floor);
	colliders_[ColliderType::Attack].IsUsing = false;
	colliders_[ColliderType::Attack].colliderName = "PlayerAttack";
}
void Player::AttackOnCollision(const ICollider& collider)
{
	if (collider.GetcollitionAttribute() == Collider::Tag::EnemyCore) {
		colliders_[ColliderType::Attack].IsUsing = false;
		//パーティクル用のベクトル
		attackVector = TransformNormal({0.0f,0.0f,1.0f},Matrix4x4 (MakeRotateMatrix(world_.transform.quaternion)));
		attackVector.Normalize();
		attackVector *= -1;
		//パーティクル生成
		AttackHitParticleEmitter.world_.transform.translate = attackColliderWorld_.transform.translate + world_.transform.translate;
		AttackHitParticleEmitter.world_.transform.translate.y += 1.0f;
		attackHitParticle_->SpawnParticle(AttackHitParticleEmitter);
		//音関連
		Audio::Stop(SEattack, true,false);
		Audio::Play(SEHitattack, 1.0f);
		//ヒットストップ
		PlayPhase::HitStop(hitStopValue);
		//コントローラー振動
		Input::VibrateController(VIBRATION_MAX, VIBRATION_MIN, vibValue);
	}
	if (collider.GetcollitionAttribute() == Collider::Tag::EnemyBall) {
		colliders_[ColliderType::Attack].IsUsing = false;
		//パーティクル用のベクトル
		attackVector = TransformNormal({ 0.0f,0.0f,1.0f }, Matrix4x4(MakeRotateMatrix(world_.transform.quaternion)));
		attackVector.Normalize();
		attackVector *= -1;
		//パーティクル生成
		AttackHitBombParticleEmitter.color = { 0.5f,0.5f,1.0f };
		AttackHitBombParticleEmitter.world_.transform.translate = attackColliderWorld_.transform.translate + world_.transform.translate;
		AttackHitBombParticleEmitter.world_.transform.translate.y += 1.0f;
		attackHitBombParticle_->SpawnParticle(AttackHitBombParticleEmitter);
		//ヒットストップ
		PlayPhase::HitStop(hitStopValue);
		//コントローラー振動
		Input::VibrateController(VIBRATION_MAX, VIBRATION_MIN, vibValue);
	}
}
#pragma endregion Collider
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
	AttackHitParticleEmitter.color.w = alpha;
	particle.color = AttackHitParticleEmitter.color;
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
	AttackHitBombParticleEmitter.color.w = alpha;
	particle.color = AttackHitBombParticleEmitter.color;
	particle.currentTime += kDeltaTime;
	particle.matWorld = MakeAffineMatrix(particle.transform.scale, Vector3{ 0.0f,0.0f,0.0f }, translate);
}
void Player::ReStert()
{
	isCompleteReStert = false;
	isDead = false;
	isDyingFlag = false;
	ChangeState<PReStert>();
}
void Player::ImGui()
{
	ImGui::Begin("Player");
	ImGui::DragFloat3("Scale", &world_.transform.scale.x);
	ImGui::DragFloat4("Rotate", &world_.transform.quaternion.x);
	ImGui::DragFloat3("Translate", &world_.transform.translate.x, 0.1f);
	ImGui::Text("%d", HP_);
	ImGui::Text(state_->ShowState().c_str());
	ImGui::End();
}
bool Player::Move()
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
		Matrix4x4 rotateMatrix = MakeRotateMatrix(Renderer::GetViewProjection().parameter.rotation_);
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
		return true;
	}
	return false;
}