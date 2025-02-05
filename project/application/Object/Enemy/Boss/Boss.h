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
#include "State/EAttackRocketPunch/EAttackRocketPunch.h"
#include "State/EDead/EDead.h"
#include "State/EDown/EDown.h"
#include "State/EReturnPosition/EReturnPosition.h"
#include "State/ERoot/ERoot.h"
#include "State/ESpawn/ESpawn.h"

#pragma endregion State
class Boss : public Enemy
{
public:
	enum ColliderType {
		WeekPoint,//弱点の当たり判定
		Arm,//腕の当たり判定
		Hund,//手の当たり判定
		END,
	};
	enum AttackState {
		Slam,//叩きつけ
		Throw,//物を投げる
		RocketPunch,//ロケットパンチ！
	};
	void Init(std::vector<Model*> models)override;
	void Update()override;
	void Draw()override;

	bool GetSlamFlag() { return isSlamFlag; };

	void SetColliderUse(int number,bool flag);
	void SetColliderAttribute(int number, uint32_t collisionAttribute);
	Animation* GetAnime() { return animationArmLDamage; };
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
	uint32_t isAttackSelect = AttackState::RocketPunch;
	//プレイヤーを追いかける関数
	bool FollowPlayer();
	//ReturnPosition
	//AttackSlamPlayer
	bool IsAttackFlag = false;
	//叩きつけ攻撃時のカメラシェイク用のフラグ
	bool isSlamFlag = false;
	//叩きつけを一回以上しているか
	bool isSlam2ndFlag = false;
	//SEハンドル
	int SEHitattack;
	//AttackThrowBomb
	std::unique_ptr<Ball> ball;
	std::unique_ptr<DummyBall> dummyBall;
	//ボールに当たった回数
	int countHitBall;
	//ダミーを発射したかどうか
	bool isThrowdummyBallFlag = false;
	//SEハンドル
	int SEthrowBall;

	//Spawn
	//寝てる演出パーティクル
	ParticleSystem* sleepParticle_;
	Emitter sleepParticleEmitter;
	void SleepUpdateParticle(Particle& particle);
	float sleepParticleValue = 2.0f;
	//Dead

	Particle CustomParticle();
	//Down
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
	std::array<OBBoxCollider,Boss::ColliderType::END> colliders_;
	WorldTransform colliderDamageWorld_;
	//ボスの攻撃の当たり判定
	void ColliderAttackInit();
	void OnCollisionAttack(const ICollider& collider);
	//指側の攻撃判定
	WorldTransform colliderAttackWorld_;

#pragma endregion Collider

	//現在のTの値
	float easeT = 0.0f;
	//raseTに毎フレーム加算する値
	float addEaseT = 0.05f;

	void AddImGui()override;
#pragma region
	Animation* animationArmLDamage;
	float animationTime_ = kDeltaTime;
#pragma endregion Animation
};
