#include "Player.h"

void Player::Initialize(std::vector<Model*> models)
{
	models_ = models;
	world_.Initialize();
	world_.transform_.scale.y = 2.0f;
	//world_.transform_.translate.y = 1.0f;
	world_.UpdateMatrix();
	input = Input::GetInstance();

	BoxCollider::Initialize();
	Collider::SetWorld(&world_);
	BoxCollider::SetSize({1.0f,1.0f,1.0f});
	SetcollitionAttribute(kCollitionAttributePlayer);
	SetcollisionMask(~kCollitionAttributePlayer);

	weapon_ = std::make_unique<Weapon>();
	weapon_->Initalize(models);
	weapon_->SetParent(world_);

}

void Player::Update()
{
	tlanslatePre = world_.transform_.translate;
#ifdef USE_IMGUI
	ImGui();
#endif
	//パッドの状態をゲット
	input->GetJoystickState(joyState);

	Move();
	PlayerRoring();

	if (behaviorRequest_) {
		//ふるまいの変更
		behavior_ = behaviorRequest_.value();
		//各ふるまいごとに初期化
		switch (behavior_)
		{
		case Behavior::kRoot:
		default:
			RootInitalize();
			weapon_->RootInit();
			break;
		case Behavior::kAttack:
			AttackInitalize();
			weapon_->AttackInit();
			break;
		}

		behaviorRequest_ = std::nullopt;
	}
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
	}

	world_.transform_.quaternion = moveQuaternion_;

	BoxCollider::Update();
	world_.UpdateMatrix();

	weapon_->Update();

	//前フレームのゲームパッドの状態を保存
	joyStatePre = joyState;
}

void Player::Draw(const ViewProjection& viewProj)
{
	weapon_->Draw(viewProj);
}

void Player::ImGui()
{
#ifdef USE_IMGUI
	ImGui::Begin("Player");
	ImGui::DragFloat3("Scale", &world_.transform_.scale.x);
	ImGui::DragFloat4("Rotate", &world_.transform_.quaternion.x);
	ImGui::DragFloat3("Translate", &world_.transform_.translate.x);
	if (ImGui::Button("Reset")) {
		world_.transform_.translate = { 0.0f,2.0f,0.0f };
		world_.transform_.quaternion = IdentityQuaternion();
		moveQuaternion_ = IdentityQuaternion();

	}
	ImGui::End();
#endif
	weapon_->ImGui();
}

void Player::OnCollision(const Collider* collider)
{
	if (collider->GetcollitionAttribute() == kCollitionAttributeFloor) {
		if (collider->GetCenter().y > world_.transform_.translate.y) {
			world_.transform_.translate.y = collider->GetCenter().y;
			world_.UpdateMatrix();
		}
	}
	else if (collider->GetcollitionAttribute() == kCollitionAttributeBox) {
		Vector3 colliderPos = collider->GetCenter();

#pragma region
		if (tlanslatePre.y - Collider::GetSize().y < colliderPos.y + collider->GetSize().y && tlanslatePre.y + Collider::GetSize().y > colliderPos.y - collider->GetSize().y) {
			if (tlanslatePre.x > colliderPos.x + collider->GetSize().x) {
				//左から右
				if (world_.transform_.translate.x - Collider::GetSize().x < colliderPos.x + collider->GetSize().x) {
					world_.transform_.translate.x = colliderPos.x + collider->GetSize().x + Collider::GetSize().x;
				}
			}
			if (tlanslatePre.x < colliderPos.x - collider->GetSize().x) {
				//右から左
				if (world_.transform_.translate.x + Collider::GetSize().x > colliderPos.x - collider->GetSize().x) {
				world_.transform_.translate.x = colliderPos.x - collider->GetSize().x - Collider::GetSize().x;
				}
			}
		}

		if (tlanslatePre.y > colliderPos.y + collider->GetSize().y) {
			//上から下
			if (world_.transform_.translate.y - Collider::GetSize().y < colliderPos.y + collider->GetSize().y) {
				world_.transform_.translate.y = colliderPos.y + collider->GetSize().y + Collider::GetSize().y;
			}
		}
		if (tlanslatePre.y < colliderPos.y - collider->GetSize().y) {
			//下から上
			if (world_.transform_.translate.y + Collider::GetSize().y > colliderPos.y - collider->GetSize().y) {
				world_.transform_.translate.y = colliderPos.y - collider->GetSize().y - Collider::GetSize().y;
			}
		}
		if (tlanslatePre.y - Collider::GetSize().y < colliderPos.y + collider->GetSize().y && tlanslatePre.y + Collider::GetSize().y > colliderPos.y - collider->GetSize().y) {
			if (tlanslatePre.z < colliderPos.z - collider->GetSize().z) {
				//手前から奥
				if (world_.transform_.translate.z + Collider::GetSize().z > colliderPos.z - collider->GetSize().z) {
					world_.transform_.translate.z = colliderPos.z - collider->GetSize().z - Collider::GetSize().z;
				}
			}
			if (tlanslatePre.z > colliderPos.z + collider->GetSize().z) {
				//奥から手前
				if (world_.transform_.translate.z - Collider::GetSize().z < colliderPos.z + collider->GetSize().z) {
					world_.transform_.translate.z = colliderPos.z + collider->GetSize().z + Collider::GetSize().z;
				}
			}
		}
#pragma endregion 移動制御	

		world_.UpdateMatrix();
	}

	if (collider->GetcollitionAttribute() == kCollitionAttributeGoal) {
		IsGoal = true;
	}
	return;
}

void Player::Move()
{
	//移動量
	if (joyState.Gamepad.sThumbLX != 0 && joyState.Gamepad.sThumbLY != 0) {
		move = {
		(float)joyState.Gamepad.sThumbLX / SHRT_MAX, 0.0f,
		(float)joyState.Gamepad.sThumbLY / SHRT_MAX };
		//正規化をして斜めの移動量を正しくする
		move = Normalize(move);
		move.x = move.x * speed;
		move.y = move.y * speed;
		move.z = move.z * speed;
		//カメラの正面方向に移動するようにする
		//回転行列を作る
		Matrix4x4 rotateMatrix = MakeRotateMatrix(viewProjection_->rotation_);
		//移動ベクトルをカメラの角度だけ回転
		move = TransformNormal(move, rotateMatrix);
		move.y = 0.0f;
		//移動
		world_.transform_.translate = Add(world_.transform_.translate, move);

	}
}

void Player::PlayerRoring()
{
	Matrix4x4 rotateMatrix = MakeRotateMatrix(viewProjection_->rotation_);
	//移動ベクトルをカメラの角度だけ回転
	lookPoint = TransformNormal({0.0f,0.0f,1.0f}, rotateMatrix);
	//ロックオン座標
	lookPoint = lookPoint + world_.transform_.translate;
	lookPoint.y = 0;
	//追従対象からロックオン対象へのベクトル
	sub = lookPoint - world_.transform_.translate;

	//プレイヤーの現在の向き
	sub = Normalize(sub);

	Vector3 cross = Normalize(Vector3::Cross({ 0.0f,0.0f,1.0f }, sub));
	float dot = Vector3::Dot({ 0.0f,0.0f,1.0f }, sub);

	//行きたい方向のQuaternionの作成
	moveQuaternion_ = MakeRotateAxisAngleQuaternion(cross, std::acos(dot));
}

void Player::RootInitalize()
{

}

void Player::RootUpdate()
{
	if (joyState.Gamepad.bRightTrigger != 0 && joyStatePre.Gamepad.bRightTrigger == 0) {
		behaviorRequest_ = Behavior::kAttack;
	}
}

void Player::AttackInitalize()
{

}

void Player::AttackUpdate()
{
	if (weapon_->GetIsAttackOver()) {
		behaviorRequest_ = Behavior::kRoot;
	}
}

void Player::Gravity()
{
	world_.transform_.translate.y -= kGravity;
}
