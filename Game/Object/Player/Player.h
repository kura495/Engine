#pragma once

#include "Object/IObject.h"
#include "Game/Object/Weapon/Weapon.h"
#include "Collider/Box/BoxCollider.h"
#include "Collider/OBB/OBBoxCollider.h"
#include "Input/Input.h"
#include "Animation/Animation.h"

enum class Behavior {
	kRoot,
	kJump,
	kAttack
};

class Player : public IObject
{
public:

	void Init(std::vector<Model*> models)override;
	void TitleUpdate();
	void Update()override;
	void TitleDraw();
	void Draw()override;

	void ImGui();

	bool GetisDead() { return isDead; };

	void Move();

private:
#pragma region 
	//ふるまい
	Behavior behavior_ = Behavior::kRoot;
	//次のふるまいリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;
	void BehaviorUpdate();
	//kRoot
	void RootInit();
	void RootUpdate();
	//kAttack
	void AttackInit();
	void AttackUpdate();
	//kJump
	void JumpInit();
	void JumpUpdate();
#pragma endregion BehaviorTree
#pragma region 
	void ColliderInit();
	void OnCollision(const ICollider* collider);
	void AttackColliderInit();
	void AttackOnCollision(const ICollider* collider);
	OBBoxCollider colliderPlayer;
	OBBoxCollider colliderAttack;
	WorldTransform attackColliderWorld_;
#pragma endregion Collider

#pragma region
	//プレイヤーの移動速度
	const float kMoveSpeed_ = 0.1f;
	//HP
	uint32_t HP_ = 10;
	//生きているか死んでいるかのフラグ
	bool isDead = false;
	//ジャンプの強さ
	const float kJumpForce = 10.0f;
	float jumpForce = 0.0f;
	//ジャンプした時の減算
	const float kJumpSubValue = 0.2f;
#pragma endregion Parameter

	Input* input = nullptr;

	XINPUT_STATE joyState;
	XINPUT_STATE joyStatePre;

	Animation* animation;
	Animation* IdleAnimation;

};