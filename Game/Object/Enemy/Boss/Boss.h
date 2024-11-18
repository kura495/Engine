#pragma once
#include "Game/Object/Enemy/Enemy.h"
#include "Animation/Animation.h"
#include "Utility/Ease/Ease.h"
#include "ParticleSystem/ParticleSystem.h"
#include "Bomb/Bomb.h"

enum Body {
	body,
	ArmL,
	ArmR
};
enum class BossBehavior {
	Root,
	ReturnPosition,//元の位置に戻る
	AttackSlamPlayer,//叩きつけ攻撃
	AttackThrowBomb,//爆弾を投げる攻撃
	Spawn,//出現時
	Dead,//死亡時
};

class Boss : public Enemy
{
public:

	void Init(std::vector<Model*> models)override;
	void Update()override;
	void Draw()override;

private:
#pragma region
	//ふるまい
	BossBehavior behavior_ = BossBehavior::Spawn;
	//次のふるまいリクエスト
	std::optional<BossBehavior> behaviorRequest_ = std::nullopt;
	void BehaviorUpdate();
	//Root
	void RootInit();
	void RootUpdate();
	//ReturnPosition
	void ReturnPositionInit();
	void ReturnPositionUpdate();
	Vector3 PrePos;
	//AttackSlamPlayer
	void AttackSlamPlayerInit();
	void AttackSlamPlayerUpdate();
	//AttackThrowBomb
	void AttackThrowBombInit();
	void AttackThrowBombUpdate();
	//Spawn
	void SpawnInit();
	void SpawnUpdate();
	//Dead
	void DeadInit();
	void DeadUpdate();
#pragma endregion Behavior
#pragma region
	bool FollowPlayer();
#pragma endregion
#pragma region
	//ボスの弱点の当たり判定
	void ColliderDamageInit();
	void OnCollision(const ICollider* colliderA)override;
	OBBoxCollider colliderDamage;
	WorldTransform colliderDamageWorld_;
	//ボスの攻撃の当たり判定
	void ColliderAttackInit();
	void OnCollisionAttack(const ICollider* collider);
	OBBoxCollider colliderAttack;
	OBBoxCollider colliderAttackA;
	WorldTransform colliderAttackWorld_;
	bool IsAttackFlag = false;
#pragma endregion Collider

#pragma region
	//現在のTの値
	float easeT = 0.0f;
	//raseTに毎フレーム加算する値
	float addEaseT = 0.05f;
#pragma endregion 攻撃に関する物

#pragma region
	
	std::unique_ptr<Bomb> bomb;

#pragma endregion 爆弾

	void AddImGui()override;

	WorldTransform worldArmL;
	//パーティクル
	ParticleSystem* particle;
	Emitter deadEnemyParticleEmitter;
	Vector3 ranpos[3] = {

	}
#pragma region
	Animation* animationArmLDamage;
	Animation* animationSpawn;
#pragma endregion Animation

};
