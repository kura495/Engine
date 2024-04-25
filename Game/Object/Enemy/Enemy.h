#pragma once

#include "Game/Object/Character.h"
#include "Base/Utility/BoxCollider.h"
#include "Base/Input/Input.h"

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

	Vector3 tlanslatePre = { 0.0f,0.0f,0.0f };

	Player* player_ = nullptr;

	bool IsDead = false;
};
