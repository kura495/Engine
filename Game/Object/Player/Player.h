#pragma once
//操作キャラクタークラス//

#include "Math/Matrix/MatrixCalc.h"
#include "Math/Vector/VectorCalc.h"
#include "Object/IObject.h"
#include "Collider/Box/BoxCollider.h"
#include "Collider/OBB/OBBoxCollider.h"
#include "Input/Input.h"
#include "Animation/Animation.h"
#include "Game/Object/Camera/FollowCamera.h"
#include "ParticleSystem/ParticleSystem.h"

enum class Behavior {
	kRoot,
	kJump,
	kAttack,
	kDead
};

class Player : public IObject
{
public:

	void Init(std::vector<Model*> models)override;
	void Update()override;
	void Draw()override;

	/// <summary>
	/// ゲームプレイシーンがタイトル状態の時
	/// </summary>
	void TitleDraw();
	void TitleUpdate();

	//HPが0になっているとtrue
	bool GetisDead() { return isDead; };

private:
	void ImGui();
	//スティック入力で移動させる関数
	void Move();
#pragma region 
	//ふるまい
	Behavior behavior_ = Behavior::kRoot;
	//次のふるまいリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;
	void BehaviorUpdate();
	//kRoot
	void RootInit();
	void RootUpdate();
	//kAttack
	void AttackInit();
	void AttackUpdate();
	//kJump
	void JumpInit();
	void JumpUpdate();
	//kDead
	void DeadInit();
	void DeadUpdate();
#pragma endregion BehaviorTree
#pragma region 
	//プレイヤーキャラ事態の当たり判定
	void ColliderInit();
	void OnCollision(const ICollider& collider);
	//攻撃の当たり判定
	void AttackColliderInit();
	void AttackOnCollision(const ICollider& collider);
	OBBoxCollider colliderPlayer;
	OBBoxCollider colliderAttack;
	WorldTransform attackColliderWorld_;
#pragma endregion Collider
#pragma region
	//プレイヤーの移動速度
	const float kMoveSpeed_ = 0.3f;
	//HP
	uint32_t HP_ = 1;
	//生きているか死んでいるかのフラグ
	bool isDead = false;
	//ジャンプの強さ
	const float kJumpForce = 0.5f;
	//ジャンプに使う実数値
	float jumpForce = 0.0f;
	//ジャンプした時の減算
	const float kJumpSubValue = 0.03f;
#pragma endregion Parameter

	Input* input = nullptr;

#pragma region
	ParticleSystem* deadParticle_;
	void UpdatedeadParticle(Particle& particle);
	Emitter deadParticleEmitter;

	ParticleSystem* attackHitParticle_;
	void UpdateAttackHitParticle(Particle& particle);
	Emitter AttackHitParticleEmitter;
	Vector3 attackVector;

	ParticleSystem* attackHitBombParticle_;
	void UpdateAttackHitBombParticle(Particle& particle);
	Emitter AttackHitBombParticleEmitter;

	const float kDeltaTime = 1.0f / 60.0f;
#pragma endregion Particle

	XINPUT_STATE joyState;
	XINPUT_STATE joyStatePre;

	Animation* walkanimation;
	Animation* attackAnimation;
	//攻撃の瞬間に向いていた姿勢を表すクォータニオン
	Quaternion attackPosture;

	Animation* deadAnimation;
	//アニメーション
	float animationTime_ = 0.0f;

	bool isDamege = false;
	//死んだときにモデルを描画するか
	bool isDeadModelDraw = true;
	//動いていたかどうか
	bool isMovedFlag = false;

	Vector3 move;
};