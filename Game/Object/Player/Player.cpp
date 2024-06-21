#include "Player.h"

bool Player::playerMoveValue;
bool Player::PushOptionButtern;

void Player::Initialize(std::vector<Model*> models)
{
	models_ = models;
	world_.Initialize();
	input = Input::GetInstance();

	moveQuaternion_ = Quaternion::IdentityQuaternion();
	//TODO : InitよりSetWorldを先にしないとWorldがNull
	ICollider::SetWorld(&world_);
	BoxCollider::Initialize();
	BoxCollider::SetSize({0.5f,1.0f,0.5f});
	SetcollitionAttribute(kCollitionAttributePlayer);
	SetcollisionMask(~kCollitionAttributePlayer);

	weapon_ = std::make_unique<Weapon>();
	weapon_->Initalize(models);
	weapon_->SetParent(world_);

#pragma region
	animation = Animation::LoadAnimationFile("resources/human", "walk.gltf");
	animation->Init();
	animation->AnimeInit(*models_[0]);

	animation2 = Animation::LoadAnimationFile("resources/human", "sneakWalk.gltf");
	animation2->Init();
	animation2->AnimeInit(*models_[0]);

	animation3 = new Animation();
	animation3->Init();
	animation3->AnimeInit(*models_[0]);
	animation3->SetSkeleton(animation->GetSkeleton(),animation->duration);
#pragma endregion Anime

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
	animation3->AnimationLerp(animation2, animation, animeT);
	animation3->PlayAnimation();

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

	if (input->pushPad(XINPUT_GAMEPAD_START) || input->pushKey(DIK_ESCAPE)) {
		if (PushOptionButtern) {
			PushOptionButtern = false;
		}
		else {
			PushOptionButtern = true;
		}
	}

	world_.UpdateMatrix();

	weapon_->Update();

	//前フレームのゲームパッドの状態を保存
	joyStatePre = joyState;
}

void Player::Draw()
{
	models_[0]->RendererSkinDraw(world_, animation3->GetSkinCluster());
	weapon_->Draw();
	animation3->DebugDraw(world_);
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
		world_.transform_.quaternion = Quaternion::IdentityQuaternion();
		moveQuaternion_ = Quaternion::IdentityQuaternion();

	}
	ImGui::End();
#endif
	weapon_->ImGui();
}

void Player::OnCollision(const ICollider* ICollider)
{
	ImGui::Begin("Collider");
	ImGui::Text("PlayerHit");
	ImGui::End();

	if (ICollider->GetcollitionAttribute() == kCollitionAttributeFloor) {
		if (ICollider->GetCenter().y > world_.transform_.translate.y) {
			world_.transform_.translate.y = ICollider->GetCenter().y;
			world_.UpdateMatrix();
		}
	}
	else if (ICollider->GetcollitionAttribute() == kCollitionAttributeBox) {
		Vector3 IColliderPos = ICollider->GetCenter();

#pragma region
		if (tlanslatePre.y - ICollider::GetSize().y < IColliderPos.y + ICollider->GetSize().y && tlanslatePre.y + ICollider::GetSize().y > IColliderPos.y - ICollider->GetSize().y) {
			if (tlanslatePre.x > IColliderPos.x + ICollider->GetSize().x) {
				//左から右
				if (world_.transform_.translate.x - ICollider::GetSize().x < IColliderPos.x + ICollider->GetSize().x) {
					world_.transform_.translate.x = IColliderPos.x + ICollider->GetSize().x + ICollider::GetSize().x;
				}
			}
			if (tlanslatePre.x < IColliderPos.x - ICollider->GetSize().x) {
				//右から左
				if (world_.transform_.translate.x + ICollider::GetSize().x > IColliderPos.x - ICollider->GetSize().x) {
				world_.transform_.translate.x = IColliderPos.x - ICollider->GetSize().x - ICollider::GetSize().x;
				}
			}
		}

		if (tlanslatePre.y > IColliderPos.y + ICollider->GetSize().y) {
			//上から下
			if (world_.transform_.translate.y - ICollider::GetSize().y < IColliderPos.y + ICollider->GetSize().y) {
				world_.transform_.translate.y = IColliderPos.y + ICollider->GetSize().y + ICollider::GetSize().y;
			}
		}
		if (tlanslatePre.y < IColliderPos.y - ICollider->GetSize().y) {
			//下から上
			if (world_.transform_.translate.y + ICollider::GetSize().y > IColliderPos.y - ICollider->GetSize().y) {
				world_.transform_.translate.y = IColliderPos.y - ICollider->GetSize().y - ICollider::GetSize().y;
			}
		}
		if (tlanslatePre.y - ICollider::GetSize().y < IColliderPos.y + ICollider->GetSize().y && tlanslatePre.y + ICollider::GetSize().y > IColliderPos.y - ICollider->GetSize().y) {
			if (tlanslatePre.z < IColliderPos.z - ICollider->GetSize().z) {
				//手前から奥
				if (world_.transform_.translate.z + ICollider::GetSize().z > IColliderPos.z - ICollider->GetSize().z) {
					world_.transform_.translate.z = IColliderPos.z - ICollider->GetSize().z - ICollider::GetSize().z;
				}
			}
			if (tlanslatePre.z > IColliderPos.z + ICollider->GetSize().z) {
				//奥から手前
				if (world_.transform_.translate.z - ICollider::GetSize().z < IColliderPos.z + ICollider->GetSize().z) {
					world_.transform_.translate.z = IColliderPos.z + ICollider->GetSize().z + ICollider::GetSize().z;
				}
			}
		}
#pragma endregion 移動制御	

		world_.UpdateMatrix();
	}

	if (ICollider->GetcollitionAttribute() == kCollitionAttributeGoal) {
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
		move = move.Normalize();
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
		world_.transform_.translate = world_.transform_.translate + move;
		playerMoveValue = true;
		//移動ベクトルをカメラの角度だけ回転
		//lookPoint = TransformNormal(move, rotateMatrix);
		//ロックオン座標
		lookPoint = move + world_.transform_.translate;
		lookPoint.y = 0;
		//追従対象からロックオン対象へのベクトル
		sub = lookPoint - world_.transform_.translate;

		//プレイヤーの現在の向き
		sub = sub.Normalize();

		Vector3 cross = Vector3::Normalize(Vector3::Cross({ 0.0f,0.0f,1.0f }, sub));
		float dot = Vector3::Dot({ 0.0f,0.0f,1.0f }, sub);

		//行きたい方向のQuaternionの作成
		moveQuaternion_ = MakeRotateAxisAngleQuaternion(cross, std::acos(dot));

		animation->PlayAnimation();
		animation2->PlayAnimation();

		animeT = (std::min)(animeT + 0.01f, 1.0f);



		return;
	}
	else {
		playerMoveValue = false;
		animeT = (std::max)(animeT - 0.01f, 0.0f);
	}
	if (input->TriggerKey(DIK_W)) {
		world_.transform_.translate.z += 0.1f;
		playerMoveValue = true;
	}
	if (input->TriggerKey(DIK_S)) {
		world_.transform_.translate.z -= 0.1f;
		playerMoveValue = true;
	}
	if (input->TriggerKey(DIK_A)) {
		world_.transform_.translate.x -= 0.1f;
		playerMoveValue = true;
	}
	if (input->TriggerKey(DIK_D)) {
		world_.transform_.translate.x += 0.1f;
		playerMoveValue = true;
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
	sub = sub.Normalize();

	Vector3 cross = Vector3::Normalize(Vector3::Cross({ 0.0f,0.0f,1.0f }, sub));
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
