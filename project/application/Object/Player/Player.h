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
		END,
	};
	void Init(std::vector<Model*> models)override;
	void Update()override;
	void Draw()override;

	//HPが0になっているとtrue
	bool GetisDead() { return isDead; };


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

	//アニメーション
	const float kgravity = 0.03f;
	float gravity = 0.03f;
	Animation* deadAnimation;
	//kAttack
	int SEattack;
	int SEHitattack;
	//kJump
	//ジャンプの強さ
	const float kJumpForce = 0.5f;
	//ジャンプに使う実数値
	float jumpForce = 0.0f;
	//ジャンプした時の減算
	const float kJumpSubValue = 0.03f;
	//kDead
		//生きているか死んでいるかのフラグ
	bool isDead = false;
	ParticleSystem* deadParticle_;
	void UpdatedeadParticle(Particle& particle);
	//TODO:準備中
	Emitter deadParticleEmitter;
	bool isDamege = false;
	//死んだときにモデルを描画するか
	bool isDeadModelDraw = true;
	//地面にいるかどうか
	bool isOnFloorFlag = true;
private:
	void ImGui();

#pragma region 
#pragma region
	std::unique_ptr<IPlayerState> state_;
#pragma endregion State

	std::array<OBBoxCollider, ColliderType::END> colliders_;
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

	ParticleSystem* attackHitParticle_;
	void UpdateAttackHitParticle(Particle& particle);
	Emitter AttackHitParticleEmitter;
	Vector3 attackVector;

	ParticleSystem* attackHitBombParticle_;
	void UpdateAttackHitBombParticle(Particle& particle);
	Emitter AttackHitBombParticleEmitter;

#pragma endregion Particle

#pragma region

#pragma endregion 音声

	XINPUT_STATE joyState;
	XINPUT_STATE joyStatePre;

	//死亡
	bool isDeadFlag = false;

	Vector3 move;
};