#pragma once
#include "Object/IObject.h"
#include "Collider/OBB/OBBoxCollider.h"
#include "Math/Vector/Vector3.h"

class Player;
class BossBullet : public IObject
{
public:
	BossBullet(Player* player) { player_ = player; };

	void Init(std::vector<Model*> models)override;
	void Update();
	void Draw();

	bool GetIsDead() { return IsDead_; }
private:
	void InitCollider();
	void OnCollision(ICollider* colliderB);

	OBBoxCollider collider;

	Player* player_;

	bool IsDead_ = false;

	float attackFlame_;
	Vector3 targetPos;
	Vector3 AddPos;
};
