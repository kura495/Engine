#include "Player.h"
#include "Game/Object/Camera/LockOn.h"

bool Player::playerMoveValue;
bool Player::PushOptionButtern;

void Player::Init(std::vector<Model*> models)
{
	name = "Player";

	models_ = models;
	world_.Initialize();
	input = Input::GetInstance();

	models_[0]->GetMaterial()->enableLighting = Lighting::NotDo;
	
	collider.Init(&world_);
	collider.SetSize({0.5f,1.0f,0.5f});
	collider.SetOffset({0.0f,0.5f,0.0f});
	collider.OnCollision = [this](ICollider* collider) { OnCollision(collider); };
	collider.SetcollitionAttribute(ColliderTag::Player);
	collider.SetcollisionMask(~ColliderTag::Player && ~ColliderTag::Weapon);

#pragma region
	animation = new Animation();
	animation = Animation::LoadAnimationFile("resources/human", "walk.gltf");
	animation->Init();
	animation->AnimeInit(*models_[0],true);
#pragma endregion Anime

	weapon_ = std::make_unique<Weapon>();
	weapon_->Initalize(models);
	weapon_->SetParent(world_);
}

void Player::Update()
{
	if (HP_ <= 0) {
		isDead = true;
	}


	tlanslatePre = world_.transform.translate;
#ifdef USE_IMGUI
	ImGui();
#endif
	//パッドの状態をゲット
	input->GetJoystickState(joyState);

	playerMoveValue = false;


	#pragma region
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
			weapon_->RootInit();
			break;
		case Behavior::kAttack:
			AttackInit();
			weapon_->AttackInit();
			break;
		case Behavior::kStep:
			StepInit();
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
		weapon_->AttackUpdate();
		break;
	case Behavior::kStep:
		StepUpdate();
		break;
	}
	#pragma endregion BehaviorTree

	//メニュー画面を開く
	if (input->pushPad(XINPUT_GAMEPAD_START) || input->pushKey(DIK_ESCAPE)) {
		if (PushOptionButtern) {
			PushOptionButtern = false;
		}
		else {
			PushOptionButtern = true;
		}
	}
	
	world_.Update();
	
	//TODO:位置を読み込んでやりたい
	//weapon_->UpdateMat(animation->GetSkeleton().joints[animation->GetSkeleton().jointMap["LeftArm"]].skeletonSpaceMatrix);
	weapon_->Update();

	//前フレームのゲームパッドの状態を保存
	joyStatePre = joyState;
}

void Player::Draw()
{
	models_[0]->RendererSkinDraw(world_, animation->GetSkinCluster());
	weapon_->Draw();
	animation->DebugDraw(world_);
}

void Player::ImGui()
{
	ImGui::Begin("Player");
	ImGui::DragFloat3("Scale", &world_.transform.scale.x);
	ImGui::DragFloat4("Rotate", &world_.transform.quaternion.x);
	ImGui::DragFloat3("Translate", &world_.transform.translate.x,0.1f);
	if (ImGui::Button("Reset")) {
		world_.transform.translate = { 0.0f,2.0f,0.0f };
		world_.transform.quaternion = Quaternion::IdentityQuaternion();
	}
	if (ImGui::Button("CollisionOn")) {
		collider.IsUsing = true;
	}
	if (ImGui::Button("CollisionOff")) {
		collider.IsUsing = false;
	}
	ImGui::Text("%d", HP_);
	ImGui::End();
	weapon_->ImGui();
}

void Player::OnCollision(const ICollider* ICollider)
{

	if (ICollider->GetcollitionAttribute() == ColliderTag::EnemyAttack) {
		HP_ -= 1;
		playerMoveValue = true;
	}

	if (ICollider->GetcollitionAttribute() == ColliderTag::Floor) {
		if (ICollider->GetCenter().y > world_.transform.translate.y) {
			world_.transform.translate.y = ICollider->GetCenter().y;
			world_.Update();
		}
	}
	else if (ICollider->GetcollitionAttribute() == ColliderTag::Box) {
		Vector3 IColliderPos = ICollider->GetCenter();

#pragma region
		if (tlanslatePre.y - collider.GetSize().y < IColliderPos.y + ICollider->GetSize().y && tlanslatePre.y + collider.GetSize().y > IColliderPos.y - ICollider->GetSize().y) {
			if (tlanslatePre.x > IColliderPos.x + ICollider->GetSize().x) {
				//左から右
				if (world_.transform.translate.x - collider.GetSize().x < IColliderPos.x + ICollider->GetSize().x) {
					world_.transform.translate.x = IColliderPos.x + ICollider->GetSize().x + collider.GetSize().x;
				}
			}
			if (tlanslatePre.x < IColliderPos.x - ICollider->GetSize().x) {
				//右から左
				if (world_.transform.translate.x + collider.GetSize().x > IColliderPos.x - ICollider->GetSize().x) {
				world_.transform.translate.x = IColliderPos.x - ICollider->GetSize().x - collider.GetSize().x;
				}
			}
		}

		if (tlanslatePre.y > IColliderPos.y + ICollider->GetSize().y) {
			//上から下
			if (world_.transform.translate.y - collider.GetSize().y < IColliderPos.y + ICollider->GetSize().y) {
				world_.transform.translate.y = IColliderPos.y + ICollider->GetSize().y + collider.GetSize().y;
			}
		}
		if (tlanslatePre.y < IColliderPos.y - ICollider->GetSize().y) {
			//下から上
			if (world_.transform.translate.y + collider.GetSize().y > IColliderPos.y - ICollider->GetSize().y) {
				world_.transform.translate.y = IColliderPos.y - ICollider->GetSize().y - collider.GetSize().y;
			}
		}
		if (tlanslatePre.y - collider.GetSize().y < IColliderPos.y + ICollider->GetSize().y && tlanslatePre.y + collider.GetSize().y > IColliderPos.y - ICollider->GetSize().y) {
			if (tlanslatePre.z < IColliderPos.z - ICollider->GetSize().z) {
				//手前から奥
				if (world_.transform.translate.z + collider.GetSize().z > IColliderPos.z - ICollider->GetSize().z) {
					world_.transform.translate.z = IColliderPos.z - ICollider->GetSize().z - collider.GetSize().z;
				}
			}
			if (tlanslatePre.z > IColliderPos.z + ICollider->GetSize().z) {
				//奥から手前
				if (world_.transform.translate.z - collider.GetSize().z < IColliderPos.z + ICollider->GetSize().z) {
					world_.transform.translate.z = IColliderPos.z + ICollider->GetSize().z + collider.GetSize().z;
				}
			}
		}
#pragma endregion 移動制御	

		world_.Update();
	}
	return;
}

void Player::Move()
{
	//移動量
	if (joyState.Gamepad.sThumbLX != 0 && joyState.Gamepad.sThumbLY != 0) {

#pragma region
		move = {
		(float)joyState.Gamepad.sThumbLX / SHRT_MAX, 0.0f,
		(float)joyState.Gamepad.sThumbLY / SHRT_MAX };
		//正規化をして斜めの移動量を正しくする
		move = move.Normalize();
		move.x = move.x * speed;
		move.y = move.y * speed;
		move.z = move.z * speed;
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
		if (!lockOn_->ExistTarget()) {
			//移動ベクトルをカメラの角度だけ回転
			//ロックオン座標
			lookPoint = move + world_.transform.translate;
			lookPoint.y = 0;
			//追従対象からロックオン対象へのベクトル
			sub = move;

			//プレイヤーの現在の向き
			sub = sub.Normalize();

			Vector3 cross = Vector3::Normalize(Vector3::Cross({ 0.0f,0.0f,1.0f }, sub));
			float dot = Vector3::Dot({ 0.0f,0.0f,1.0f }, sub);

			//行きたい方向のQuaternionの作成
			world_.transform.quaternion = Quaternion::MakeRotateAxisAngleQuaternion(cross, std::acos(dot));

		}
		else if (lockOn_ && lockOn_->ExistTarget()) {
		//ロックオン座標
		Vector3 lockOnPosition = lockOn_->GetTargetPosition();
		lockOnPosition.y = 0;
		//追従対象からロックオン対象へのベクトル
		sub = lockOnPosition - world_.GetTranslateFromMatWorld();

		//プレイヤーの現在の向き
		sub = sub.Normalize();

		Vector3 cross = Vector3::Normalize(Vector3::Cross({ 0.0f,0.0f,1.0f }, sub));
		float dot = Vector3::Dot({ 0.0f,0.0f,1.0f }, sub);

		//行きたい方向のQuaternionの作成
		world_.transform.quaternion = Quaternion::MakeRotateAxisAngleQuaternion(cross, std::acos(dot));
		}

#pragma endregion プレイヤーの回転

		animation->PlayAnimation();

		return;
	}
	

}
#pragma region
//kRoot
void Player::RootInit()
{
	collider.IsUsing = true;
}
void Player::RootUpdate()
{
	Move();

	if (input->GetPadPrecede(XINPUT_GAMEPAD_RIGHT_SHOULDER, 20)) {
		behaviorRequest_ = Behavior::kAttack;
	}

	if (input->GetPadPrecede(XINPUT_GAMEPAD_A,20)) {
		behaviorRequest_ = Behavior::kStep;
	}
}
//kAttack
void Player::AttackInit()
{

}
void Player::AttackUpdate()
{
	if (weapon_->GetIsAttackOver()) {
		if (input->GetPadPrecede(XINPUT_GAMEPAD_A, 3)) {
			behaviorRequest_ = Behavior::kStep;
			return;
		}
		behaviorRequest_ = Behavior::kRoot;
	}
}
//kStep
void Player::StepInit(){
	collider.IsUsing = false;
	IsEndStep = false;
	stepFlame = 0.0f;
}
void Player::StepUpdate(){
	//動いているかどうかで分岐
	if (playerMoveValue) {
		//ローリング回避
		Rolling();
	}
	else {
	//後ろに回避
	BackStep();
	}

	stepFlame++;
	if (stepFlame > 5.0f) {
		//終わりを知らせる
		IsEndStep = true;
	}

	if (IsEndStep) {
		//Behaviorを戻す
		behaviorRequest_ = Behavior::kRoot;
	}
}
void Player::Rolling()
{
	Vector3 StepMoveValue = { 0.0f,0.0f,kStepValue };
	//カメラの正面方向に移動するようにする
	//回転行列を作る
	Matrix4x4 rotateMatrix = MakeRotateMatrix(world_.transform.quaternion);
	//移動ベクトルをカメラの角度だけ回転
	StepMoveValue = TransformNormal(StepMoveValue, rotateMatrix);
	StepMoveValue.y = 0.0f;
	//移動
	world_.transform.translate += StepMoveValue;
}
void Player::BackStep()
{
	Vector3 StepMoveValue = { 0.0f,0.0f,kStepValue };
	//カメラの正面方向に移動するようにする
	//回転行列を作る
	Matrix4x4 rotateMatrix = MakeRotateMatrix(world_.transform.quaternion);
	//移動ベクトルをカメラの角度だけ回転
	StepMoveValue = TransformNormal(StepMoveValue, rotateMatrix);
	StepMoveValue.y = 0.0f;
	//移動
	world_.transform.translate -= StepMoveValue;
}
#pragma endregion BeheviorTree