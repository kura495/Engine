#pragma once
#include "Game/Object/Enemy/Enemy.h"
#include "Animation/Animation.h"

#include "Utility/Ease/Ease.h"

enum Body {
	body,
	ArmL,
	ArmR
};
enum class BossBehavior {
	kRoot,
	kAttackL,
	kAttackR,
};

class Boss : public Enemy
{
public:

	void Init(std::vector<Model*> models)override;
	void Update()override;
	void Draw()override;

private:
#pragma region
	//ふるまい
	BossBehavior behavior_ = BossBehavior::kRoot;
	//次のふるまいリクエスト
	std::optional<BossBehavior> behaviorRequest_ = std::nullopt;
	void BehaviorUpdate();
	//kRoot
	void kRootInit();
	void kRootUpdate();
	//kAttackL
	void kAttackLInit();
	void kAttackLUpdate();
	//kAttackR
	void kAttackRInit();
	void kAttackRUpdate();
#pragma endregion Behavior
#pragma region
	//ボスの弱点の当たり判定
	void colliderDamageInit();
	void OnCollision(const ICollider* colliderA)override;
	OBBoxCollider colliderDamage;
	WorldTransform ColliderDamegeWorld_;
	//ボスの攻撃の当たり判定
	void colliderAttackInit();
	void onCollisionAttack(const ICollider* collider);
	OBBoxCollider colliderAttack;
	WorldTransform colliderAttackWorld_;
#pragma endregion Collider

#pragma region
	//現在のTの値
	float easeT = 0.0f;
	//raseTに毎フレーム加算する値
	float addEaseT = 0.05f;
#pragma endregion 攻撃に関する物

	void AddImGui()override;

	WorldTransform worldArmL;
	//WorldTransform worldArmR;

#pragma region

	Animation* animationBodykRoot;

	Animation* animationArmLRoot;
	//Animation* animationArmRRoot;
	Animation* animationArmLDamage;
	//Animation* animationArmRDamage;

	Animation* IdleAnimation;
#pragma endregion Animation

};
