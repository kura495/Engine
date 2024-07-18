#pragma once

#include "Base/Object/IObject.h"
#include "Base/Collider/Box/BoxCollider.h"
#include "Base/Collider/OBB/OBBoxCollider.h"
#include "Base/Input/Input.h"
#include "Base/Animation/Animation.h"

class Player;

class Enemy : public IObject,public OBBoxCollider
{
public:
	void Init(std::vector<Model*> models)override;
	void Update()override;
	void Draw()override;

	void ImGui();

	void OnCollision(const ICollider* collider);

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
