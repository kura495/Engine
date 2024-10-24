#pragma once
//操作キャラクタークラス//

#include "Object/IObject.h"
#include "Collider/Box/BoxCollider.h"
#include "Collider/OBB/OBBoxCollider.h"
#include "Input/Input.h"
#include "Animation/Animation.h"
#include "Game/Object/Camera/FollowCamera.h"

enum class Behavior {
	kRoot,
	kJump,
	kAttack
};

class Player : public IObject
{
public:

	void Init(std::vector<Model*> models)override;
	void Update()override;
	void Draw()override;
	
	/// <summary>
	/// ゲームプレイシーンがタイトル状態の時
	/// </summary>
	void TitleDraw();
	void TitleUpdate();

	//HPが0になっているとtrue
	bool GetisDead() { return isDead; };

	void SetCamera(FollowCamera* camera) { followCamera = camera; };
private:
	void ImGui();
	void Move();
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
	//プレイヤーキャラ事態の当たり判定
	void ColliderInit();
	void OnCollision(const ICollider* collider);
	//攻撃の当たり判定
	void AttackColliderInit();
	void AttackOnCollision(const ICollider* collider);
	OBBoxCollider colliderPlayer;
	OBBoxCollider colliderAttack;
	WorldTransform attackColliderWorld_;
#pragma endregion Collider
#pragma region
	//プレイヤーの移動速度
	const float kMoveSpeed_ = 0.3f;
	//HP
	uint32_t HP_ = 10;
	//生きているか死んでいるかのフラグ
	bool isDead = false;
	//ジャンプの強さ
	const float kJumpForce = 0.5f;
	//ジャンプに使う実数値
	float jumpForce = 0.0f;
	//ジャンプした時の減算
	const float kJumpSubValue = 0.03f;
#pragma endregion Parameter

#pragma region

	FollowCamera* followCamera;
	//一定幅開くとカメラが近くなる
	const float kMax = 5.0f;
#pragma endregion

	Input* input = nullptr;


	XINPUT_STATE joyState;
	XINPUT_STATE joyStatePre;

	Animation* animation;
	Animation* IdleAnimation;

};