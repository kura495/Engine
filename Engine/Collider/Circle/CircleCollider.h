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

	Shape GetShape() override {
		return Shape::Circle;
	};

private:

};

CircleCollider::CircleCollider()
{
}

CircleCollider::~CircleCollider()
{
}