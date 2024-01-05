#include "BoxCollider.h"

void BoxCollider::Initialize()
{

	Collider::SetId(ColliderType::Box);
}

void BoxCollider::Update()
{
	range_.x = std::abs(range_.x);
	range_.y = std::abs(range_.y);
	range_.z = std::abs(range_.z);
	//小さいポイントの設定
	aabb_.min.x = center_->GetTranslateFromMatWorld().x - range_.x / 2;
	aabb_.min.y = center_->GetTranslateFromMatWorld().y - range_.y / 2;
	aabb_.min.z = center_->GetTranslateFromMatWorld().z - range_.z / 2;
	//大きいポイントの設定
	aabb_.max.x = center_->GetTranslateFromMatWorld().x + range_.x / 2;
	aabb_.max.y = center_->GetTranslateFromMatWorld().y + range_.y / 2;
	aabb_.max.z = center_->GetTranslateFromMatWorld().z + range_.z / 2;
}

