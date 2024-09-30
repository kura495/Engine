#pragma once
#include "Game/Object/Enemy/Enemy.h"
#include "Game/Object/Enemy/BossSpider/Bullet/BossBullet.h"

enum class BossSpiderBehavior {
	kRoot,
	kAttack
};

class BossSpider : public Enemy {
public:

	void Init(std::vector<Model*> models)override;
	void Update()override;
	void Draw()override;

private:
	void InitCollider();

	void ImGui();
#pragma region
	//ふるまい
	BossSpiderBehavior behavior_ = BossSpiderBehavior::kRoot;
	//次のふるまいリクエスト
	std::optional<BossSpiderBehavior> behaviorRequest_ = std::nullopt;

	void RootInit();
	void RootUpdate();

	void AttackInit();
	void AttackUpdate();

#pragma endregion Behavior

	void LookPlayer();

	std::list<BossBullet*> bossBullet_;

	float attackFlame_;
	float kAttackFlame_ = 120.0f;
};