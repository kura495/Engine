#pragma once
#include "Model/Model.h"
#include "WorldTransform/WorldTransform.h"
#include "Collider/OBB/OBBoxCollider.h"
#include "Player/Player.h"
class BeamSword
{
public:
	void Init();
	void Update();
	void Draw();
	void SetParent(WorldTransform* parent) { world_.parent_ = parent; };
	void SetModelSize(Vector3 size);
private:
	//FollowPlayerの速度の定数
	const float kFollowPlayerSpeed = 0.5f;
	//FollowPlayerがtrueを返す一定距離
	const float kConstantDistance = 0.2f;

	std::unique_ptr<Model>model_;
	WorldTransform world_;
	OBBoxCollider collider_;

	void OnCollision(ICollider& Collider);

	Math::Vector3 angle;

	bool IsAttack = false;

	Math::Vector3 Csize = { 1.0f,0.5f,25.0f };
	Math::Vector3 Coffset = { 0.0f,0.0f,-24.0f };
};

