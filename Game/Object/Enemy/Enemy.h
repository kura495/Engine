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

	void PlayAnime();

	void SetPlayer(Player* player) { player_ = player; };
	void SetPos(Vector3 pos) {
		world_.transform.translate = pos;
		world_.UpdateMatrix();
	};
	Vector3 GetPos() {
		return world_.transform.translate;
	}

	bool GetIsAlive() {
		return IsAlive;
	}

private://関数

	void ImGui();
	void OnCollision(const ICollider* ICollider);
	void AttackOnCollision(const ICollider* ICollider);
	// 攻撃範囲内ならtrue
	void AttackInit();
	bool ChackOnAttack();
	void ChasePlayer();
	void LookPlayer();

private://変数
	Player* player_ = nullptr;
	Animation* animation = nullptr;

	bool IsAlive = true;

	uint32_t HP_ = 10;

	const float AttackRange = 2.0f;
	bool isAttackFlag = false;
	bool attackColliderFlag = true;
	WorldTransform attackWorld_;

	OBBoxCollider collider;
	OBBoxCollider attackCollider;

	float animationTime_ = 0.0f;
	float animeInterval_ = 60.0f;

	Quaternion PreQua;
};
