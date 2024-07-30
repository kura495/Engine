#pragma once

#include "Base/Object/IObject.h"
#include "Base/Collider/Box/BoxCollider.h"
#include "Base/Collider/OBB/OBBoxCollider.h"
#include "Base/Input/Input.h"
#include "Base/Animation/Animation.h"

class Player;

class Enemy : public IObject
{
public:
	void Init(std::vector<Model*> models)override;
	void Update()override;
	void Draw()override;

	void SetPlayer(Player* player) { player_ = player; };
	void SetPos(Vector3 pos) {
		world_.transform.translate = pos;
		world_.UpdateMatrix();
	};
private://関数

	void ImGui();
	void OnCollision(const ICollider* ICollider);
	// 攻撃範囲内ならtrue
	bool ChackOnAttack();
	void ChasePlayer();

private://変数
	Player* player_ = nullptr;
	Animation* animation = nullptr;

	uint32_t HP_ = 10;

	const float AttackRange = 2.0f;
	bool isAttackFlag = true;

	OBBoxCollider collider;
};
