#pragma once
#include "Collider.h"
class CircleCollider : public Collider
{
public:
	CircleCollider();
	~CircleCollider();

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