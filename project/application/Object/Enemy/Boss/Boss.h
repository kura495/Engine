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
#include "State/ESwordAttack/ESwordAttack.h"
#include "State/EReStert/EReStert.h"
#pragma endregion State
class Boss : public Enemy
{
public:
	enum ColliderType {
		DamageArm,//腕の当たり判定(ダメージ用)
		DamageHund,//手の当たり判定(ダメージ用)
		AttackArm,//腕の当たり判定(攻撃用)
		AttackHund,//手の当たり判定(攻撃用)
		ColliderTypeEND,
	};
	enum BossModel {
		MainBody,//ボスのメインモデル
		BossModelEND,
	};
	void Init(std::vector<Model*> models)override;
	void Update()override;
	void Draw()override;
	/// <summary>
	/// リスタートの時に呼び出す処理
	/// </summary>
	void ReStert();

	FollowCamera* getCamera() { return camera_; };
	void SetCamera(FollowCamera* camera) { camera_ = camera; };

	bool GetSlamFlag() { return isSlamFlag; };
	/// <summary>
	/// コライダーON,OFFの切り替え
	/// </summary>
	/// <param name="number">コライダー要素番号</param>
	/// <param name="flag">bool型</param>
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

	//AttackSlam
	//叩きつけ攻撃時のカメラシェイク用のフラグ
	bool isSlamFlag = false;

	#pragma endregion State
private:

#pragma region
	std::unique_ptr<IBossState> state_;
#pragma endregion State

#pragma region
	void ColliderInit();
	//ボスの弱点の当たり判定
	void ColliderDamageInit();
	void OnCollisionDamage(const ICollider& collider);
	//ボスの攻撃の当たり判定
	void ColliderAttackInit();
	void OnCollisionAttack(const ICollider& collider);
	std::array<OBBoxCollider,Boss::ColliderType::ColliderTypeEND> colliders_;
	//コライダー共通の座標
	WorldTransform colliderWorld_;
	//腕の当たり判定の値
	Vector3 armColliderSize = { 1.0f,1.0f,7.0f };
	Vector3 armColliderOffset = { 0.0f,0.0f,-2.0f };
	//手の当たり判定の値
	Vector3 hundColliderSize = { 2.0f,0.5f,1.0f };
	Vector3 hundColliderOffset = { 0.0f,0.0f,-6.25f };
#pragma endregion Collider

	void AddImGui()override;
#pragma region
	std::unique_ptr<Animation> animationArmLDamage;
	float animationTime_ = kDeltaTime;
#pragma endregion Animation
	std::vector<float> items;
	std::string name;

	FollowCamera* camera_;
};
