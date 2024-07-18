#pragma once
#include "Math_Structs.h"
#include "CollisionConfig.h"
#include "WorldTransform/WorldTransform.h"
#include <cstdint>

static uint32_t CollisionNumber = 0;

class ICollider {
public:

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

	using HitFunction = std::function<void(ICollider*)>;
	HitFunction OnCollision;
	//衝突時に呼ばれる関数
	
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

	///worldの親を設定
	void SetWorld(WorldTransform* Parent) { center_ = Parent; }

	Vector3 GetCenter() const {
		return
		{ center_->transform_.translate.x,center_->transform_.translate.y,center_->transform_.translate.z };
	}
	void SetCenter(Vector3 input) {
		centerPoint = input;
	}


	void SetSize(Vector3 size) { size_ = size; }
	Vector3 GetSize() const { return size_; }

	virtual Shape GetShape() = 0;
	//
	bool IsUsing = true;
protected:
	/// <summary>
	/// 原点
	/// </summary>
	WorldTransform* center_ = nullptr;
	Vector3 centerPoint;
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
