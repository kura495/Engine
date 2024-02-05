#pragma once

#include "Game/Object/Character.h"
#include "Base/Utility/BoxCollider.h"
#include "Base/Input/Input.h"
#include "Game/Object/Enemy/SearchPoint/SearchPoint.h"

class Player;

class Enemy : public Character, public BoxCollider
{
public:
	void Initialize(std::vector<Model*> models)override;
	void Update()override;
	void Draw(const ViewProjection& viewProj)override;

	void ImGui();

	void OnCollision(const Collider* collider)override;

	void SetPlayer(Player* player) { player_ = player; };
	void SetPos(Vector3 pos) {
		world_.transform_.translate = pos;
		world_.UpdateMatrix();
	};

	SearchPoint* GetSearchPoint() {
		return searchPoint.get();
	}

private:
	std::unique_ptr<SearchPoint>searchPoint;

	Vector3 tlanslatePre;

	Player* player_;

	bool IsDead = false;
};
