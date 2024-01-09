#pragma once

#include "Game/Object/Character.h"
#include "Game/Object/Weapon/Weapon.h"
#include "Base/Utility/BoxCollider.h"
#include "Base/Input/Input.h"

class Player : public Character	, public BoxCollider
{
public:

	void Initalize(std::vector<Model*> models)override;
	void Update()override;
	void Draw(const ViewProjection& viewProj)override;

	void ImGui()override;

	void OnCollision(const Collider* collider)override;

	void Move();

#pragma region Setter
	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
		weapon_->SetViewProjection(viewProjection_);
	}
#pragma endregion
#pragma region
	const WorldTransform& GetWorldTransform() {
		return world_;
	}
#pragma endregion

private:
	Input* input = nullptr;

	void Gravity();
	const float kGravity = 0.98f;
	bool IsGravity = true;

	XINPUT_STATE joyState;
	XINPUT_STATE joyStatePre;

	Vector3 move;
	//プレイヤーの移動
	float speed = 0.3f;

	Vector3 jumpForce;
	//武器や腕の回転クォータニオン
	Quaternion moveQuaternion_;

	//カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

	Vector3 tlanslatePre;

	std::unique_ptr<Weapon>weapon_;

};