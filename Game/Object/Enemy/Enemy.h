#pragma once

#include "Object/IObject.h"
#include "Collider/Box/BoxCollider.h"
#include "Collider/OBB/OBBoxCollider.h"
#include "Input/Input.h"
#include "Animation/Animation.h"

class Player;

class Enemy : public IObject
{
public:


	void SetPlayer(Player* player) { player_ = player; };
	void SetPos(Vector3 pos) {
		world_.transform.translate = pos;
		world_.Update();
	};
	Vector3 GetPos() {
		return world_.transform.translate;
	}

	bool GetIsAlive() {
		return IsAlive;
	}

protected://関数

	void ImGui();

	virtual void OnCollision(const ICollider* ICollider);

protected:

	OBBoxCollider collider;

	Animation* animation = nullptr;

	const float AttackRange = 2.0f;
	bool isAttackFlag = false;
	bool attackColliderFlag = true;

	float animationTime_ = 0.0f;
	float animeInterval_ = 60.0f;

	Quaternion PreQua;

	bool isSpawn = true;
	int spawnFlame_ = 0;
	const int kSpawnFlame_ = 60;

	bool isDamege = false;
	int damegeInterval = 0;
	const int kDamegeInterval = 60;

	uint32_t HP_;

	bool IsAlive = true;

	Player* player_ = nullptr;

};
