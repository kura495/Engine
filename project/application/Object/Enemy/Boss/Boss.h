#pragma once
/*ボスクラス*/
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

enum Body {
	body,
	ArmL,
	ArmR
};
enum class BossBehavior {
	Root,
	ReturnPosition,//元の位置に戻る
	AttackSlamPlayer,//叩きつけ攻撃
	AttackThrowball,//爆弾を投げる攻撃
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
	void RootInit();
	void RootUpdate();
	void RootDraw();
	//ReturnPosition
	void ReturnPositionInit();
	void ReturnPositionUpdate();
	void ReturnPositionDraw();
	//AttackSlamPlayer
	void AttackSlamInit();
	void AttackSlamUpdate();
	void AttackSlamDraw();
	//AttackThrowBomb
	void AttackThrowBallInit();
	void AttackThrowBallUpdate();
	void AttackThrowBallDraw();
	//Spawn
	void SpawnInit();
	void SpawnUpdate();
	void SpawnDraw();
	//Dead
	void DeadInit();
	void DeadUpdate();
	void DeadDraw();
	//Down
	void DownInit();
	void DownUpdate();
	void DownDraw();
	#pragma endregion State
private:

#pragma region
	std::unique_ptr<IBossState> state_;
//Root
	
//ReturnPosition
	//戻る位置を保存する変数
	Vector3 PrePos;
//AttackSlamPlayer
	//叩きつけ攻撃時のカメラシェイク用のフラグ
	bool isSlamFlag = false;
	//叩きつけを一回以上しているか
	bool isSlam2ndFlag = false;
//AttackThrowBomb
	std::unique_ptr<Ball> ball;
	std::unique_ptr<DummyBall> dummyBall;
	//ボールに当たった回数
	int countHitBall;
	//ダミーを発射したかどうか
	bool isThrowdummyBallFlag = false;
//Spawn
	//寝てる演出パーティクル
	ParticleSystem* sleepParticle_;
	Emitter sleepParticleEmitter;
	void SleepUpdateParticle(Particle& particle);
	float sleepParticleValue = 2.0f;
//Dead
	//死亡パーティクル
	ParticleSystem* deadParticle_;
	Emitter deadEnemyParticleEmitter;
	void UpdateParticle(Particle& particle);
	Particle CustomParticle();
//Down
	
#pragma endregion State

#pragma region
	//ボスの弱点の当たり判定
	void ColliderDamageInit();
	void OnCollision(const ICollider& colliderA)override;
	OBBoxCollider colliderDamage;
	WorldTransform colliderDamageWorld_;
	//ボスの攻撃の当たり判定
	void ColliderAttackInit();
	void OnCollisionAttack(const ICollider& collider);
	//腕側の攻撃判定
	OBBoxCollider colliderAttack;
	//指側の攻撃判定
	OBBoxCollider colliderAttackA;
	WorldTransform colliderAttackWorld_;
	bool IsAttackFlag = false;
#pragma endregion Collider

	//現在のTの値
	float easeT = 0.0f;
	//raseTに毎フレーム加算する値
	float addEaseT = 0.05f;

#pragma region
	Animation* animationArmLDamage;
#pragma endregion Animation
#pragma region Down
	Vector3 DownPosition{ 0.0f,0.5f,20.0f };
	bool isDownStert;
	int hitCount = 0;
#pragma endregion
#pragma region
	Audio* SEPlayer;
	int SEthrowBall;
	int SEHitattack;
#pragma endregion 音声
	bool FollowPlayer();
	void AddImGui()override;

	WorldTransform worldArmL;

	//固定の位置
	Vector3 initialPosition{ 0.0f,5.5f,35.0f };

	bool isAttackSelect = true;


};
