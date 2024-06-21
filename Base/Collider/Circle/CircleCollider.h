#pragma once
#include "Base/Collider/ICollider.h"
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