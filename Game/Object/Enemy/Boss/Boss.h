#pragma once
#include "Game/Object/Enemy/Enemy.h"
#include "Game/Object/Enemy/Boss/Weapon/BossWeapon.h"
enum class BossBehavior {
	kRoot,
	kAttack,
	kParry,
	kSpown,
	kDying,
};
enum class AttackBehavior {
	kAttack1,
	kAttack2,
};

class Boss : public Enemy
{
public:
	Boss();
	~Boss();

	void Init(std::vector<Model*> models)override;
	void Update()override;
	void BehaviorUpdate();
	void Draw()override;

private:
	void LookPlayer();

	std::unique_ptr<BossWeapon> bossWeapon_;

#pragma region
	//ふるまい
	BossBehavior behavior_ = BossBehavior::kRoot;
	//次のふるまいリクエスト
	std::optional<BossBehavior> behaviorRequest_ = std::nullopt;
	//kRoot
	void RootInit();
	void RootUpdate();
	//kAttack
	void AttackInit();
	void AttackUpdate();
	//kParry
	void ParryInit();
	void ParryUpdate();
	//kSpown
	void SpownInit();
	void SpownUpdate();
	//kDying
	void DyingInit();
	void DyingUpdate();
#pragma endregion Behavior
#pragma region
	//ふるまい
	AttackBehavior attackBehavior_ = AttackBehavior::kAttack1;
	//次のふるまいリクエスト
	std::optional<AttackBehavior> attackbehaviorRequest_ = std::nullopt;
	void AttackBehaviorUpdate();
	//kAttack1
	void Attack1Init();
	void Attack1Update();
	//kAttack2
	void Attack2Init();
	void Attack2Update();
#pragma endregion AttackBehavior
#pragma region
	void InitCollider();
#pragma endregion Collider

};

