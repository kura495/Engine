#pragma once

#include "Game/Object/Character.h"
#include "Base/Utility/BoxCollider.h"
#include "Base/Input/Input.h"
#include "Base/Animation/Animation.h"

class Player;

class Enemy : public Character
{
public:
	void Initialize(std::vector<Model*> models)override;
	void Update()override;
	void Draw(const ViewProjection& viewProj)override;

	void ImGui();

	void SetPlayer(Player* player) { player_ = player; };
	void SetPos(Vector3 pos) {
		world_.transform_.translate = pos;
		world_.UpdateMatrix();
	};

private:
	Player* player_ = nullptr;
	Animation animation;
	// 攻撃範囲内ならtrue
	bool ChackOnAttack();
	void ChasePlayer();

	const float AttackRange = 3.0f;
	bool isAttackFlag = true;

	float animationTime_ = 0.0f;
	const float kAnimeInterval = 60.0f;
	float animeInterval_ = 0.0f;

	//SkinAnime
	Skeleton skeleton;
	SkinCluster skinCluster;
};
