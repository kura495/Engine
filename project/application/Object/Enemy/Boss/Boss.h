#pragma once
/*ボスクラス*/
#include <vector>

#include "Object/Enemy/Enemy.h"
#include "Animation/Animation.h"
#include "Utility/Ease/Ease.h"
#include "ParticleSystem/ParticleSystem.h"

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
		ColliderTypeEND,
	};
	enum AttackState {
		Slam,//叩きつけ
		Throw,//物を投げる
		RocketPunch,//ロケットパンチ！
	};
	enum BossModel {
		MainBody,//ボスのメインモデル
		PlayerModelEND,
	};
	void Init(std::vector<Model*> models)override;
	void Update()override;
	void Draw()override;

	bool GetSlamFlag() { return isSlamFlag; };

	void SetColliderUse(int number,bool flag);
	void SetColliderAttribute(int number, uint32_t collisionAttribute);
	Animation* GetAnime() { return animationArmLDamage.get(); };
	/// <summary>
	/// ステートを切り替える
	/// </summary>
	/// ステートを切り替えたタイミングで前が破棄されてしまうため、タイミングは注意。
	/// ChangeStateの後に更新を入れない、もしくはreturnで返す
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
	//AttackSlam
	//叩きつけ攻撃時のカメラシェイク用のフラグ
	bool isSlamFlag = false;

	#pragma endregion State
private:

#pragma region
	std::unique_ptr<IBossState> state_;

#pragma endregion State

#pragma region
	//ボスの弱点の当たり判定
	void ColliderDamageInit();
	void OnCollision(const ICollider& colliderA)override;
	std::array<OBBoxCollider,Boss::ColliderType::ColliderTypeEND> colliders_;
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
	std::unique_ptr<Animation> animationArmLDamage;
	float animationTime_ = kDeltaTime;
#pragma endregion Animation
	//FollowPlayerの速度の定数
	const float kFollowPlayerSpeed = 0.5f;
	//FollowPlayerがtrueを返す一定距離
	const float kConstantDistance = 0.2f;
};
