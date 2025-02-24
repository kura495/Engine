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
	void Init(std::vector<Model*> models)override;
	void Update()override;
	void Draw()override;

	//HPが0になっているとtrue
	bool GetisDead() { return isDead; };
	bool GetisDying() { return isDyingFlag; };

	void SetColliderUse(int number, bool flag);
	void SetColliderAttribute(int number, uint32_t collisionAttribute);

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
	//kRoot
	//スティック入力で移動させる関数
	bool Move();
	//kDead
	//生きているか死んでいるかのフラグ
	bool isDead = false;

private:
	void ImGui();

	bool isDamege = false;

	//地面にいるかどうか
	bool isOnFloorFlag = true;
#pragma region
	std::unique_ptr<IPlayerState> state_;
#pragma endregion State

	const float kgravity = 0.03f;
	float gravity = 0.03f;

#pragma region 

	//当たり判定
	std::array<OBBoxCollider, ColliderType::ColliderTypeEND> colliders_;
	WorldTransform attackColliderWorld_;
	//プレイヤーキャラ事態の当たり判定
	void ColliderInit();
	void OnCollision(const ICollider& collider);
	//攻撃の当たり判定
	void AttackColliderInit();
	void AttackOnCollision(const ICollider& collider);

#pragma endregion Collider

#pragma region
	//プレイヤーの移動速度
	const float kMoveSpeed_ = 0.3f;
	//HP
	uint32_t HP_ = 1;

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

	Vector3 move;
};