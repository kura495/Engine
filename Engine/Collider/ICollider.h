#pragma once
//当たり判定基底クラス//
#include "Math_Structs.h"
#include "CollisionConfig.h"
#include "WorldTransform/WorldTransform.h"
#include <cstdint>
using namespace Math;
static uint32_t CollisionNumber = 0;

class ICollider {
public:

	bool IsDalete = false;
	//コライダーの種類
	enum Shape {
		None,//セットされていない
		Box,
		OBB,
		Capsule,
		Sphere,
		Circle,
		MaxValue
	};
	ICollider() {
		id = CollisionNumber;
		CollisionNumber++;
	};
	virtual ~ICollider(){};

	virtual void CollisionUpdate() = 0;
	virtual void CollisionDraw() = 0;
	//押し戻し用
	Vector3 pushForce;

	//TODO：共通のImGuiをまとめたものを作りたい
	/*virtual void ColliderImGui(std::string TabName) {
	
	};*/
	//関数ポインタ　OnCollisionを入れる
	using HitFunction = std::function<void(ICollider&)>;
	HitFunction OnCollision;
#pragma region
	/// <summary>
	/// 衝突時に呼ばれる関数
	/// コライダーの属性とマスク設定
	/// </summary>
	/// <returns></returns>
	uint32_t GetcollitionAttribute() const { return collisionAttribute_; }
	/// <summary>
	/// 衝突属性
	/// コライダーの持つ属性を設定
	/// </summary>
	/// <param name="collisionAttribute">コライダーの持つ属性</param>
	void SetcollitionAttribute(uint32_t collisionAttribute) {
		collisionAttribute_ = collisionAttribute;
	}
	uint32_t GetcollisionMask() const { return collisionMask_; }
	/// <summary>
	/// 衝突マスク
	/// 判定を取りたくないコライダータイプを設定
	/// </summary>
	/// <param name="collisionMask">基本はSetcollitionAttributeに入れたものに~を付けてビット反転</param>
	void SetcollisionMask(uint32_t collisionMask){

		collisionMask_ = collisionMask;
	}
#pragma endregion Attribute

	/// <summary>
	/// 当たり判定のワールドトランスフォームを設定
	/// </summary>
	/// <param name="world">設定するワールドトランスフォーム</param>
	void SetWorld(WorldTransform* world) { center_ = world; }
	/// <summary>
	/// 当たり判定の位置を取得
	/// </summary>
	/// <returns>Vector3</returns>
	Vector3 GetCenter() const {
		return
		{ center_->transform.translate.x,center_->transform.translate.y,center_->transform.translate.z };
	}
	/// <summary>
	/// 当たり判定のワールドトランスフォームを取得
	/// </summary>
	/// <returns>WorldTransform*</returns>
	WorldTransform* GetColliderWorld() { return center_; };
	/// <summary>
	/// コライダーの当たり判定のオフセットを設定
	/// </summary>
	/// <param name="input"></param>
	void SetOffset(Vector3 input) {
		offset = input;
	}

	/// <summary>
	/// 当たり判定の大きさを設定
	/// </summary>
	/// <param name="size"></param>
	void SetSize(Vector3 size) { size_ = size; }
	/// <summary>
	/// 当たり判定の大きさを取得
	/// </summary>
	/// <returns>Vector3</returns>
	Vector3 GetSize() const { return size_; }
	/// <summary>
	/// 形状の判別をする関数
	/// </summary>
	/// <returns>Shape</returns>
	virtual Shape GetShape() = 0;

	//コライダー有効化フラグ(trueなら当たり判定をとる)
	bool IsUsing = true;
	//コライダーの名前
	std::string colliderName = "collider";
protected:
	/// <summary>
	/// 原点
	/// </summary>
	WorldTransform* center_ = nullptr;
	Vector3 offset;
	/// <summary>
	/// x,y,zそれぞれの幅
	/// </summary>
	Vector3 size_{0};

private:

	/// <summary>
	/// 衝突属性(自分)
	/// </summary>
	uint32_t collisionAttribute_ = 0xffffffff;
	/// <summary>
	/// 衝突マスク(相手)
	/// </summary>
	uint32_t collisionMask_ = 0xffffffff;

	uint32_t id;

};
