#pragma once
#include "Math_Structs.h"
#include "CollisionConfig.h"
#include "WorldTransform/WorldTransform.h"
#include <cstdint>

static uint32_t CollisionNumber = 0;

class ICollider {
public:

	bool IsDalete = false;

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

	//TODO：共通のImGuiをまとめたものを作りたい
	/*virtual void ColliderImGui(std::string TabName) {
	
	};*/

	using HitFunction = std::function<void(ICollider*)>;
	HitFunction OnCollision;
	//衝突時に呼ばれる関数
	//コライダーの属性とマスク設定
#pragma region
	uint32_t GetcollitionAttribute() const { return collisionAttribute_; }
	/// <summary>
	/// 衝突属性
	/// </summary>
	/// <param name="collisionAttribute"></param>
	void SetcollitionAttribute(uint32_t collisionAttribute) {
		collisionAttribute_ = collisionAttribute;
	}
	uint32_t GetcollisionMask() const { return collisionMask_; }
	/// <summary>
	/// 衝突マスク
	/// </summary>
	/// <param name="collisionMask">基本はSetcollitionAttributeに入れたものに~を付けてビット反転</param>
	void SetcollisionMask(uint32_t collisionMask){

		collisionMask_ = collisionMask;
	}
#pragma endregion Attribute

	///worldを設定
	void SetWorld(WorldTransform* world) { center_ = world; }

	Vector3 GetCenter() const {
		return
		{ center_->transform.translate.x,center_->transform.translate.y,center_->transform.translate.z };
	}
	WorldTransform* GetColliderWorld() { return center_; };

	void SetOffset(Vector3 input) {
		offset = input;
	}


	void SetSize(Vector3 size) { size_ = size; }
	Vector3 GetSize() const { return size_; }
	//形状の判別
	virtual Shape GetShape() = 0;

	//使うかどうかのフラグ
	bool IsUsing = true;
	//名前
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
