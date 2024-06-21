#include "BoxCollider.h"

void BoxCollider::Initialize()
{
	model_ = std::make_unique<BoxColliderModel>();
	model_->Init();
	model_->SetTranslate(center_->transform_.translate);
	model_->SetScale(size_);
}

void BoxCollider::CollisionUpdate()
{
	size_.x = std::abs(size_.x);
	size_.y = std::abs(size_.y);
	size_.z = std::abs(size_.z);
	//小さいポイントの設定
	aabb_.min.x = center_->GetTranslateFromMatWorld().x - size_.x + centerPoint.x;
	aabb_.min.y = center_->GetTranslateFromMatWorld().y - size_.y + centerPoint.y;
	aabb_.min.z = center_->GetTranslateFromMatWorld().z - size_.z + centerPoint.z;
	//大きいポイントの設定
	aabb_.max.x = center_->GetTranslateFromMatWorld().x + size_.x + centerPoint.x;
	aabb_.max.y = center_->GetTranslateFromMatWorld().y + size_.y + centerPoint.y;
	aabb_.max.z = center_->GetTranslateFromMatWorld().z + size_.z + centerPoint.z;
	model_->SetTranslate(center_->transform_.translate);
	model_->SetScale(size_);
	model_->Update();
}

void BoxCollider::CollisionDraw()
{
	model_->Draw();
}

