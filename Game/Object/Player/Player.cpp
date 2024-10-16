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
	animation = new Animation();
	animation = Animation::LoadAnimationFile("resources/human", "walk.gltf");
	animation->Init();
	animation->AnimeInit(*models_[0], true);

	IdleAnimation = new Animation();
	IdleAnimation = Animation::LoadAnimationFile("resources/human", "Idle.gltf");
	IdleAnimation->Init();
	IdleAnimation->AnimeInit(*models_[0], true);
#pragma endregion Anime
}

void Player::TitleUpdate()
{
	IdleAnimation->PlayAnimation();
}

void Player::Update()
{

	//パッドの状態をゲット
	input->GetJoystickState(joyState);

	if (HP_ <= 0) {
		isDead = true;
	}
	Move();

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

	world_.Update();

	//前フレームのゲームパッドの状態を保存
	joyStatePre = joyState;
}

void Player::TitleDraw()
{
	models_[0]->RendererSkinDraw(world_, IdleAnimation->GetSkinCluster());
}

void Player::Draw()
{
#ifdef _DEBUG
	animation->DebugDraw(world_);
#endif
	models_[0]->RendererSkinDraw(world_, animation->GetSkinCluster());
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
	
		animation->PlayAnimation();

		return;
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
	}
}
//kRoot
void Player::RootInit()
{
	colliderPlayer.IsUsing = true;
}
void Player::RootUpdate()
{
	//攻撃
	if (input->GetPadPrecede(XINPUT_GAMEPAD_X, 20)) {
		behaviorRequest_ = Behavior::kAttack;
	}
	//ジャンプ
	else if (input->GetPadPrecede(XINPUT_GAMEPAD_A, 20)) {
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
	colliderPlayer.IsUsing = false;
	jumpForce = kJumpForce;
}
void Player::JumpUpdate() {
	world_.transform.translate.y += jumpForce;
	jumpForce -= kJumpSubValue;

	if (world_.transform.translate.y <= 0) {
		world_.transform.translate.y = 0;
		behaviorRequest_ = Behavior::kRoot;
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
		HP_ -= 1;
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
	attackColliderWorld_.transform.translate.y = 0.5f;
	attackColliderWorld_.transform.translate.z = 1.0f;
	colliderAttack.Init(&attackColliderWorld_);
	colliderAttack.SetSize({ 0.5f,1.0f,0.5f });
	colliderAttack.OnCollision = [this](ICollider* collider) { AttackOnCollision(collider); };
	colliderAttack.SetcollitionAttribute(ColliderTag::Player);
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