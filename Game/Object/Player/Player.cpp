#include "Player.h"

void Player::Initalize(std::vector<Model*> models)
{
	models_ = models;
	world_.Initialize();
	world_.transform_.translate.y = 1.0f;
	world_.transform_.scale.y = 2.0f;
	world_.UpdateMatrix();
	input = Input::GetInstance();

	BoxCollider::Initialize();
	Collider::SetWorld(&world_);
	BoxCollider::SetSize({0.5f,1.0f,0.5f});
	SetcollitionAttribute(kCollitionAttributePlayer);
	SetcollisionMask(~kCollitionAttributePlayer);

}

void Player::Update()
{
	tlanslatePre = world_.transform_.translate;
#ifdef _DEBUG
	ImGui();
#endif
	//パッドの状態をゲット
	input->GetJoystickState(joyState);

	Move();

	BoxCollider::Update();
	world_.UpdateMatrix();

	//前フレームのゲームパッドの状態を保存
	joyStatePre = joyState;
}

void Player::Draw(const ViewProjection& viewProj)
{
	for (Model* model : models_) {
		model->Draw(world_, viewProj);
	}

}

void Player::ImGui()
{
#ifdef _DEBUG
	ImGui::Begin("Player");
	ImGui::DragFloat3("Scale", &world_.transform_.scale.x);
	ImGui::DragFloat4("Rotate", &world_.transform_.quaternion.x);
	ImGui::DragFloat3("Translate", &world_.transform_.translate.x);
	if (ImGui::Button("Reset")) {
		world_.transform_.translate = { 0.0f,1.0f,0.0f };
	}
	ImGui::End();
#endif
}

void Player::OnCollision(const Collider* collider)
{
	if (collider->GetcollitionAttribute() == kCollitionAttributeFloor) {
#ifdef _DEBUG
		ImGui::Begin("FloorHit");
		ImGui::End();
#endif
		if (collider->GetCenter().y > world_.transform_.translate.y) {
			world_.transform_.translate.y = collider->GetCenter().y;
			world_.UpdateMatrix();
		}
	}
	else if (collider->GetcollitionAttribute() == kCollitionAttributeBox) {
#ifdef _DEBUG
		ImGui::Begin("BoxHit");
		ImGui::End();
#endif

		Vector3 colliderPos = collider->GetCenter();

#pragma region
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
#pragma endregion 移動制御	

		world_.UpdateMatrix();
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
		//プレイヤーの向きを移動方向に合わせる
		move = Normalize(move);
		move.y = 0.0f;
		Vector3 cross = Normalize(Cross({ 0.0f,0.0f,1.0f }, move));
		float dot = Dot({ 0.0f,0.0f,1.0f }, move);
		moveQuaternion_ = MakeRotateAxisAngleQuaternion(cross, std::acos(dot));

	}
}

void Player::Gravity()
{
	world_.transform_.translate.y -= kGravity;
}
