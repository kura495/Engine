#pragma once
//操作キャラクタークラス//
#include "Object/IObject.h"
#include "Math/Matrix/MatrixCalc.h"
#include "Audio/Audio.h"
#include "Collider/Box/BoxCollider.h"
#include "Collider/OBB/OBBoxCollider.h"
#include "Input/Input.h"
#include "Animation/Animation.h"
#include "Object/Camera/FollowCamera.h"
#include "ParticleSystem/ParticleSystem.h"

#pragma region
#include "State/PAttack/PAttack.h"
#include "State/PDead/PDead.h"
#include "State/PJump/PJump.h"
#include "State/PRoot/PRoot.h"
#include "State/PReStert/PReStert.h"
#include "State/IPlayerState.h"
#pragma endregion State


class Player : public IObject
{
public:
	enum ColliderType {
		pCollider,//プレイヤーの当たり判定
		Attack,//攻撃判定
		ColliderTypeEND,
	};
	enum PlayerModel {
		MainBody,//プレイヤーのメインモデル
		PlayerModelEND,
	};
	enum CauseOfDeath {
		Normal,//通常の死
		Slam,//叩きつけられた
		Ball,//ボールに衝突
		Slash,//斬られる
		END,
	};

	void Init(std::vector<Model*> models)override;
	void Update()override;
	void Draw()override;

	//HPが0になっているとtrue
	int GetHP() { return HP_; }
	bool GetisDead() { return isDead; };
	bool GetisDying() { return isDyingFlag; };

	void SetColliderUse(int number, bool flag);
	void SetColliderAttribute(int number, uint32_t collisionAttribute);
	uint8_t GetCauseOfDeath()const { return causeOfDeath_; }
	PlayerState GetState() const { return state_->GetStateType(); };
	/// <summary>
	/// ステートを切り替える
	/// </summary>
	template <typename T>
	void ChangeState() {
		state_ = std::make_unique<T>();
		//ステートのすべての処理が終わる前にステートを切り替えてしまうと、
		//ステートが変わって後の処理が出来なくなってエラーが出る
		state_->Init(this);
	}
	//スティック入力で移動させる関数
	bool Move();
	//生きているか死んでいるかのフラグ
	bool isDead = false;
	//リスタート処理が完了したかどうか
	bool isCompleteReStert = false;
	void ReStert();
private:
	Vector3 uvT;

	void ImGui();

	bool isDamege = false;
	//地面にいるかどうか
	bool isOnFloorFlag = true;
#pragma region
	std::unique_ptr<IPlayerState> state_;
#pragma endregion State

	const float maxGravity = 0.98f;
	const float kgravity = 0.03f;
	float gravity = 0.03f;

#pragma region 

	//当たり判定
	std::array<OBBoxCollider, ColliderType::ColliderTypeEND> colliders_;
	WorldTransform attackColliderWorld_;
	//プレイヤーキャラ事態の当たり判定
	void ColliderInit();
	void OnCollision(const ICollider& collider);
	Vector3 colliderSize = { 0.5f,0.7f,0.5f };
	Vector3 colliderOffset = { 0.0f,0.7f,0.0f };
	//攻撃の当たり判定
	void AttackColliderInit();
	void AttackOnCollision(const ICollider& collider);
	Vector3 attackColliderOffset = { 0.0f,0.5f,1.0f };
	float hitStopValue = 0.2f;
	float vibValue = 0.2f;
#pragma endregion Collider

#pragma region
	//プレイヤーの移動速度
	const float kMoveSpeed_ = 0.3f;
	//HP
	uint8_t HP_ = 3;

#pragma endregion Parameter

	Input* input = nullptr;

#pragma region

	std::unique_ptr<ParticleSystem> attackHitParticle_;
	void UpdateAttackHitParticle(Particle& particle);
	Emitter AttackHitParticleEmitter;
	Vector3 attackVector;

	std::unique_ptr<ParticleSystem> attackHitBombParticle_;
	void UpdateAttackHitBombParticle(Particle& particle);
	Emitter AttackHitBombParticleEmitter;

#pragma endregion Particle

#pragma region
	//音声
	int SEattack;
	int SEHitattack;
#pragma endregion 音声

	XINPUT_STATE joyState;

	//死亡
	bool isDyingFlag = false;
	//死因
	uint8_t causeOfDeath_;

	Vector3 move;

	std::vector<Vector3> pushForce;
};