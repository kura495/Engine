#pragma once

#include "Game/Object/Character.h"
#include "Game/Object/Weapon/Weapon.h"
#include "Base/Collider/Box/BoxCollider.h"
#include "Base/Collider/OBB/OBBoxCollider.h"
#include "Base/Input/Input.h"
#include "Base/Animation/Animation.h"

enum class Behavior {
	kRoot,
	kAttack,
};

class Player : public Character	, public OBBoxCollider
{
public:

	void Initialize(std::vector<Model*> models)override;
	void Update()override;
	void Draw()override;

	void ImGui();

	void OnCollision(const ICollider* collider)override;

	void Move();
	void PlayerRoring();
	static bool playerMoveValue;
	static bool PushOptionButtern;

#pragma region Setter
	void SetViewProjection(const ViewProjection* viewProjection) {
		viewProjection_ = viewProjection;
		weapon_->SetViewProjection(viewProjection_);
	}
#pragma endregion 
#pragma region Getter
	WorldTransform& GetWorldTransform() {
		return world_;
	}
	Weapon* GetWeapon() {
		return weapon_.get();
	}
	bool GetIsGoal() {
		return IsGoal;
	}
#pragma endregion

private:
	Input* input = nullptr;

	bool IsGoal = false;

	//ふるまい
	Behavior behavior_ = Behavior::kRoot;
	//次のふるまいリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	void RootInitalize();
	void RootUpdate();

	void AttackInitalize();
	void AttackUpdate();

	void Gravity();
	const float kGravity = 0.98f;
	bool IsGravity = true;

	XINPUT_STATE joyState;
	XINPUT_STATE joyStatePre;

	Vector3 lookPoint;
	Vector3 sub;

	Vector3 move;
	//プレイヤーの移動
	float speed = 0.1f;

	Vector3 jumpForce{0.0f};
	//武器や腕の回転クォータニオン
	Quaternion moveQuaternion_{0.0f};

	//カメラのビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;

	Vector3 tlanslatePre = { 0.0f,0.0f,0.0f };

	std::unique_ptr<Weapon> weapon_;

	float attack = 0.0f;

	float animeT = 0.0f;
	Animation* animation;
	Animation* animation2;
	Animation* animation3;
};