#pragma once
#include "Game/Object/Enemy/Enemy.h"

enum class SpiderBehavior {
	kRoot,
	kAttack,
	kStep
};

class Spider : public Enemy {
public:

	void Init(std::vector<Model*> models)override;
	void Update()override;
	void Draw()override;

private://関数
#pragma region
	//ふるまい
	SpiderBehavior behavior_ = SpiderBehavior::kRoot;
	//次のふるまいリクエスト
	std::optional<SpiderBehavior> behaviorRequest_ = std::nullopt;
#pragma endregion Behavior

	void InitCollider();

	// 攻撃範囲内ならtrue
	void AttackInit();
	void PlayAttackAnime();
	bool ChackOnAttack();
	void ChasePlayer();
	void LookPlayer();

	void DamageEffect();

	void AttackOnCollision(const ICollider* ICollider);

	OBBoxCollider attackCollider;
	WorldTransform attackWorld_;
};