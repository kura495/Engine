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
	walkanimation = Animation::LoadAnimationFile("resources/human", "walk.gltf");
	walkanimation->Init();
	walkanimation->AnimeInit(*models_[0], true);

	idleAnimation = new Animation();
	idleAnimation = Animation::LoadAnimationFile("resources/human", "Idle.gltf");
	idleAnimation->Init();
	idleAnimation->AnimeInit(*models_[0], true);

	deadAnimation = new Animation();
	deadAnimation = Animation::LoadAnimationFile("resources/human", "dead.gltf");
	deadAnimation->Init();
	deadAnimation->AnimeInit(*models_[0], true);
#pragma endregion Anime

#pragma region
	particle = new ParticleSystem();
	particle->Initalize("resources/circle2.png");

	deadParticleEmitter.count = 10;
	deadParticleEmitter.frequency = 100;
	deadParticleEmitter.frequencyTime = 100;
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

	if (isDamege) {
		behaviorRequest_ = Behavior::kDead;
		isDamege = false;
		HP_ -= 1;
	}

	BehaviorUpdate();

#ifdef USE_IMGUI
	ImGui();
#endif

	//メニュー画面を開く
	//if (input->pushPad(XINPUT_GAMEPAD_START) || input->pushKey(DIK_ESCAPE)) {
	//	if (PushOptionButtern) {
	//		PushOptionButtern = false;
	//	}
	//	else {
	//		PushOptionButtern = true;
	//	}
	//}
	/*float leng = followCamera->GetViewProjection().translation_.y - world_.transform.translate.y;
	if (leng > kMax) {

	}*/
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
	models_[0]->RendererSkinDraw(world_, idleAnimation->GetSkinCluster());
}

void Player::Draw()
{
#ifdef _DEBUG
	walkanimation->DebugDraw(world_);
#endif

	switch (behavior_)
	{
	case Behavior::kRoot:
	default:
		models_[0]->RendererSkinDraw(world_, walkanimation->GetSkinCluster());
		break;
	case Behavior::kAttack:
		break;
	case Behavior::kJump:
		models_[0]->RendererSkinDraw(world_, walkanimation->GetSkinCluster());
		break;
	case Behavior::kDead:
		if (isDeadModelDraw) {
			models_[0]->RendererSkinDraw(world_, deadAnimation->GetSkinCluster());
		}
		else {
			particle->RendererDraw();
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
	FollowCamera::workInter.interParameter_.y = 0.0f;

}
void Player::RootUpdate()
{
	Move();
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
}
void Player::AttackUpdate()
{
	//kRootに戻す
	behaviorRequest_ = Behavior::kRoot;
}
//kJump
void Player::JumpInit() {
	jumpForce = kJumpForce;
	FollowCamera::workInter.interParameter_.y = 0.0f;
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

void Player::DeadInit()
{
	deadParticleEmitter.world_.transform.translate = world_.transform.translate;
	deadParticleEmitter.world_.transform.translate.y += 1.0f;
}

void Player::DeadUpdate()
{
	deadAnimation->PlayAnimation();

	animationTime_ += 2.0f / 60.0f;
	if (animationTime_ > deadAnimation->duration) {
		isDamege = false;
		animationTime_ = 0.0f;
		//behaviorRequest_ = Behavior::kRoot;
		if (HP_ <= 0) {
			isDeadModelDraw = false;
			isDead = true;
		}
	}
	if (isDeadModelDraw == false) {
		particle->Update(deadParticleEmitter);
	}
}

#pragma endregion BeheviorTree
#pragma region
void Player::ColliderInit()
{
	colliderPlayer.Init(&world_);
	colliderPlayer.SetSize({ 0.5f,1.0f,0.5f });
	colliderPlayer.SetOffset({ 0.0f,0.5f,0.0f });
	colliderPlayer.OnCollision = [this](ICollider* collider) { OnCollision(collider); };
	colliderPlayer.SetcollitionAttribute(ColliderTag::Player);
	colliderPlayer.SetcollisionMask(~ColliderTag::Player && ~ColliderTag::Weapon);
}
void Player::OnCollision(const ICollider* ICollider)
{

	if (ICollider->GetcollitionAttribute() == ColliderTag::EnemyAttack) {
		isDamege = true;
		ImGui::Begin("Player");
		ImGui::Text("Hit");
		ImGui::End();
		//playerMoveValue = true;
	}
	if (ICollider->GetcollitionAttribute() == ColliderTag::Floor) {
		if (ICollider->GetCenter().y > world_.transform.translate.y) {
			world_.transform.translate.y = ICollider->GetCenter().y;
			world_.Update();
		}
	}
	else if (ICollider->GetcollitionAttribute() == ColliderTag::Box) {
		Vector3 IColliderPos = ICollider->GetCenter();

		world_.Update();
	}
	return;
}
void Player::AttackColliderInit()
{
	attackColliderWorld_.SetParent(&world_);
	colliderAttack.Init(&attackColliderWorld_);
	colliderAttack.SetSize({ 0.5f,1.0f,0.5f });
	colliderAttack.SetOffset({ 0.0f,0.5f,1.0f });
	colliderAttack.OnCollision = [this](ICollider* collider) { AttackOnCollision(collider); };
	colliderAttack.SetcollitionAttribute(ColliderTag::Weapon);
	colliderAttack.SetcollisionMask(~ColliderTag::Player && ~ColliderTag::Weapon);

	colliderAttack.IsUsing = false;
}
void Player::AttackOnCollision(const ICollider* collider)
{
	if (collider->GetcollitionAttribute() == ColliderTag::Enemy) {
		colliderAttack.IsUsing = false;
	}
}
#pragma endregion Collider

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

		walkanimation->PlayAnimation();

		return;
	}


}