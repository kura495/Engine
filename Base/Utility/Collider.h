﻿#pragma once
#include "Math_Structs.h"
#include "CollisionConfig.h"
#include "WorldTransform/WorldTransform.h"
#include <cstdint>

class Collider {
public:

	virtual ~Collider(){};
	//衝突時に呼ばれる関数
	virtual void OnCollision(const Collider* collider) = 0;
	float GetRadius() { return Radius_; }

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

	int GetId() { return Id_; }

	///worldの親を設定
	void SetWorld(WorldTransform* Parent) { center_ = Parent; }

	Vector3 GetCenter() const {
		return
		{ center_->transform_.translate.x,center_->transform_.translate.y,center_->transform_.translate.z };
	}


	void SetSize(Vector3 size) { size_ = size; }
	Vector3 GetSize() const { return size_; }

protected:

	void SetId(int Type) { Id_ = Type; }
	/// <summary>
	/// 原点
	/// </summary>
	WorldTransform* center_;
	
	/// <summary>
/// x,y,zそれぞれの幅
/// </summary>
	Vector3 size_;

private:

	uint32_t Id_ = 0xffffffff;


	/// <summary>
	/// 衝突属性(自分)
	/// </summary>
	uint32_t collisionAttribute_ = 0xffffffff;
	/// <summary>
	/// 衝突マスク(相手)
	/// </summary>
	uint32_t collisionMask_ = 0xffffffff;

	float Radius_;


};
