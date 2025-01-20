#pragma once
//円型コライダー(Circle)//
//TODO:未完成
#include "Collider/ICollider.h"
class CircleCollider : public ICollider
{
public:
	CircleCollider();
	~CircleCollider();

	void CollisionUpdate()override;
	void CollisionDraw()override;
	/// <summary>
	/// 形状の判別をする関数
	/// </summary>
	/// <returns>Shape</returns>
	Shape GetShape() override {
		return Shape::Circle;
	};

private:

};