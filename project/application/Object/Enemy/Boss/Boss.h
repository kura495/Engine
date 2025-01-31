#pragma once
/*ボスクラス*/
#include <vector>

#include "Object/Enemy/Enemy.h"
#include "Animation/Animation.h"
#include "Utility/Ease/Ease.h"
#include "ParticleSystem/ParticleSystem.h"
#include "Ball/Ball.h"
#include "DummyBall/DummyBall.h"
#include "Engine/Audio/Audio.h"
#pragma region

#include "State/EAttackSlam/EAttackSlam.h"
#include "State/EAttackThrowball/EAttackThrowball.h"
#include "State/EDead/EDead.h"
#include "State/EDown/EDown.h"
#include "State/EReturnPosition/EReturnPosition.h"
#include "State/ERoot/ERoot.h"
#include "State/ESpawn/ESpawn.h"

#pragma endregion State
enum class BossBehavior {
	Root,
	ReturnPosition,//元の位置に戻る
	AttackSlamPlayer,//叩きつけ攻撃
	AttackThrowball,//爆弾を投げる攻撃
	Spawn,//出現時
	Dead,//死亡時
	Down,//ダウン時
};
enum ColliderNumber {
	WeekPoint,//弱点の当たり判定
	Arm,//腕の当たり判定
	Hund,//手の当たり判定
	END,
};
class Boss : public Enemy
{
public:

	void Init(std::vector<Model*> models)override;
	void Update()override;
	void Draw()override;

	bool GetSlamFlag() { return isSlamFlag; };

	void SetColliderUse(int number,bool flag);
	void SetColliderAttribute(int number, uint32_t collisionAttribute);
	/// <summary>
	/// ステートを切り替える
	/// </summary>
	template <typename T>
	void ChangeState() {
		state_ = std::make_unique<T>();
		state_->Init(this);
	}
	#pragma region
	//Root
	//攻撃の選択をする
	bool isAttackSelect = true;
	//プレイヤーを追いかける関数
	bool FollowPlayer();
	//ReturnPosition
	void ReturnPositionInit();
	void ReturnPositionUpdate();
	void ReturnPositionDraw();
	//戻る前の位置を保存する変数
	Vector3 PrePos;
	//固定の位置
	Vector3 initialPosition{ 0.0f,5.5f,35.0f };
	//AttackSlamPlayer
	void AttackSlamInit();
	void AttackSlamUpdate();
	void AttackSlamDraw();
	//叩きつけ攻撃時のカメラシェイク用のフラグ
	bool isSlamFlag = false;
	//叩きつけを一回以上しているか
	bool isSlam2ndFlag = false;
	//SEハンドル
	int SEHitattack;
	//AttackThrowBomb
	void AttackThrowBallInit();
	void AttackThrowBallUpdate();
	void AttackThrowBallDraw();
	std::unique_ptr<Ball> ball;
	std::unique_ptr<DummyBall> dummyBall;
	//ボールに当たった回数
	int countHitBall;
	//ダミーを発射したかどうか
	bool isThrowdummyBallFlag = false;
	//SEハンドル
	int SEthrowBall;
	//AttackRocketPunch
	void RocketPunchInit();
	void RocketPunchUpdate();
	void RocketPunchDraw();
	//ターゲットに向かうベクトル
	Vector3 forTargetVector;
	//Spawn
	void SpawnInit();
	void SpawnUpdate();
	void SpawnDraw();
	//寝てる演出パーティクル
	ParticleSystem* sleepParticle_;
	Emitter sleepParticleEmitter;
	void SleepUpdateParticle(Particle& particle);
	float sleepParticleValue = 2.0f;
	//Dead
	void DeadInit();
	void DeadUpdate();
	void DeadDraw();
	//死亡パーティクル
	ParticleSystem* deadParticle_;
	Emitter deadEnemyParticleEmitter;
	void UpdateParticle(Particle& particle);
	Particle CustomParticle();
	//Down
	void DownInit();
	void DownUpdate();
	void DownDraw();
	Vector3 DownPosition{ 0.0f,0.5f,20.0f };
	bool isDownStert;
	int hitCount = 0;
	#pragma endregion State
private:

#pragma region
	std::unique_ptr<IBossState> state_;

#pragma endregion State

#pragma region
	//ボスの弱点の当たり判定
	void ColliderDamageInit();
	void OnCollision(const ICollider& colliderA)override;
	std::array<OBBoxCollider,3> colliders_;
	WorldTransform colliderDamageWorld_;
	//ボスの攻撃の当たり判定
	void ColliderAttackInit();
	void OnCollisionAttack(const ICollider& collider);
	//指側の攻撃判定
	WorldTransform colliderAttackWorld_;
	bool IsAttackFlag = false;
#pragma endregion Collider


	void AddImGui()override;
#pragma region
	Animation* animationArmLDamage;
#pragma endregion Animation
};
