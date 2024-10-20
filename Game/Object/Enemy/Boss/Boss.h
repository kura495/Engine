#pragma once
#include "Game/Object/Enemy/Enemy.h"
#include "Animation/Animation.h"

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
	//プレイヤーキャラ事態の当たり判定
	//
	void InitCollider();

	void colliderDamageInit();
	void OnCollision(const ICollider* colliderA)override;
	//攻撃の当たり判定
	void colliderAttackInit();
	void onCollisionAttack(const ICollider* collider);
	OBBoxCollider colliderDamage;
	OBBoxCollider colliderAttack;
	WorldTransform ColliderDamegeWorld_;
	WorldTransform colliderAttackWorld_;

#pragma endregion Collider

	WorldTransform worldArmL;
	//WorldTransform worldArmR;

	void Damage();

	int HP_ = 10;

#pragma region

	Animation* animationBodykRoot;

	Animation* animationArmLRoot;
	//Animation* animationArmRRoot;
	Animation* animationArmLDamage;
	//Animation* animationArmRDamage;


	Animation* IdleAnimation;
#pragma endregion Animation
};
