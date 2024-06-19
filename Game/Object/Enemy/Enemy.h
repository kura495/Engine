#pragma once

#include "Game/Object/Character.h"
#include "Base/Utility/BoxCollider.h"
#include "Base/Input/Input.h"
#include "Base/Animation/Animation.h"

class Player;

class Enemy : public Character,public BoxCollider
{
public:
	void Initialize(std::vector<Model*> models)override;
	void Update()override;
	void Draw()override;

	void OnCollision(const Collider* collider)override;

	void SetPlayer(Player* player) { player_ = player; };
	void SetPos(Vector3 pos) {
		world_.transform_.translate = pos;
		world_.UpdateMatrix();
	};

private:
	Player* player_ = nullptr;
	Animation* animation = nullptr;
	// 攻撃範囲内ならtrue
	bool ChackOnAttack();
	void ChasePlayer();

	const float AttackRange = 2.0f;
	bool isAttackFlag = true;

	const float kAnimeInterval = 60.0f;
	float animeInterval_ = 0.0f;

	bool chackBoxflag = true;
};
