#include "OBBoxCollider.h"

void OBBoxCollider::Init(WorldTransform* Parent)
{
	ICollider::SetWorld(Parent);
	model_ = std::make_unique<OBBColliderModel>();
	model_->Init();
	model_->SetWorld(size_, center_->transform_.quaternion, center_->transform_.translate);
}

void OBBoxCollider::CollisionUpdate()
{
	obb_.center = center_->transform_.translate;
	obb_.size[0] = size_.x;
	obb_.size[1] = size_.y;
	obb_.size[2] = size_.z;
	SetOrientations(MakeRotateMatrix(center_->transform_.quaternion));
	model_->SetWorld(size_, center_->transform_.quaternion,center_->transform_.translate);
	model_->Update();
}

void OBBoxCollider::CollisionDraw()
{
	model_->Draw();
}

void OBBoxCollider::SetOrientations(Matrix4x4 Matrix)
{
	//X成分
	obb_.orientations[0] = GetXAxis(Matrix);
	obb_.orientations[1] = GetYAxis(Matrix);
	obb_.orientations[2] = GetZAxis(Matrix);

}
