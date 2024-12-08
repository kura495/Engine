#pragma once
#include "Game/Object/Enemy/Enemy.h"
#include "Animation/Animation.h"
#include "Utility/Ease/Ease.h"
#include "ParticleSystem/ParticleSystem.h"
#include "Bomb/Bomb.h"
#include "DummyBomb/DummyBomb.h"

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
	Down,//ダウン時
};

class Boss : public Enemy
{
public:

	void Init(std::vector<Model*> models)override;
	void Update()override;
	void Draw()override;

	bool GetSlamFlag() { return isSlamFlag; };
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
	//Down
	void DownInit();
	void DownUpdate();
#pragma endregion Behavior
#pragma region
	bool FollowPlayer();
#pragma endregion
#pragma region
	//ボスの弱点の当たり判定
	void ColliderDamageInit();
	void OnCollision(const ICollider& colliderA)override;
	OBBoxCollider colliderDamage;
	WorldTransform colliderDamageWorld_;
	//ボスの攻撃の当たり判定
	void ColliderAttackInit();
	void OnCollisionAttack(const ICollider& collider);
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
	//叩きつけ攻撃時のカメラシェイク用のフラグ
	bool isSlamFlag = false;
	//叩きつけを一回以上しているか
	bool isSlam2ndFlag = false;
	//ダミーを発射したかどうか
	bool isThrowDummyBombFlag = false;
#pragma endregion 攻撃に関する物

#pragma region
	
	std::unique_ptr<Bomb> bomb;

	std::unique_ptr<DummyBomb> dummyBomb;
	//爆弾に当たった回数
	int countHitBomb;

#pragma endregion 爆弾

#pragma region
	void AddImGui()override;

	WorldTransform worldArmL;
	//パーティクル
	ParticleSystem* particle_;
	Emitter deadEnemyParticleEmitter;
	void UpdateParticle(Particle& particle);
	Particle CustomParticle();

#pragma endregion パーティクル

#pragma region
	Animation* animationArmLDamage;
	Animation* animationSpawn;
#pragma endregion Animation

	//固定の位置
	Vector3 initialPosition{ 0.0f,5.5f,35.0f };

#pragma region Down
	Vector3 DownPosition{ 0.0f,0.5f,20.0f };
	bool isDownStert;
	int hitCount = 0;
#pragma endregion

	bool isAttackSelect = true;

	const float kDeltaTime = 1.0f / 60.0f;
};
