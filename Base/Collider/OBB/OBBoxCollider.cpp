#include "OBBoxCollider.h"
#include "Collider/Manager/CollisionManager.h"
OBBoxCollider::OBBoxCollider()
{
	CollisionManager::AddCollider(this);
}

OBBoxCollider::~OBBoxCollider()
{
	IsDalete = true;
}

void OBBoxCollider::Init(WorldTransform* Parent)
{
	ICollider::SetWorld(Parent);
	model_ = std::make_unique<OBBColliderModel>();
	model_->Init();
	model_->SetWorld(size_, center_->transform.quaternion, center_->transform.translate);
}

void OBBoxCollider::CollisionUpdate()
{
	Vector3 Pos = center_->transform.translate;
	Vector3 offsetVec = offset;
	Quaternion qua = center_->transform.quaternion;
	SetOrientations(MakeRotateMatrix(center_->transform.quaternion));
	if (center_->parent_) {
		offsetVec = center_->transform.translate;
		offsetVec = TransformNormal(offsetVec, MakeRotateMatrix(center_->parent_->transform.quaternion));
		//移動
		SetOrientations(MakeRotateMatrix(center_->parent_->transform.quaternion * center_->transform.quaternion));
		qua = center_->parent_->transform.quaternion * center_->transform.quaternion;
		Pos = center_->parent_->GetTranslateFromMatWorld();
	}
	obb_.center = Pos + offsetVec;
	obb_.size[0] = size_.x;
	obb_.size[1] = size_.y;
	obb_.size[2] = size_.z;

	model_->SetWorld(size_, qua, obb_.center);
	model_->Update();
}

void OBBoxCollider::CollisionDraw()
{
	if (IsUsing) {
		model_->Draw();
	}
}

void OBBoxCollider::SetOrientations(Matrix4x4 Matrix)
{
	//X成分
	obb_.orientations[0] = GetXAxis(Matrix);
	obb_.orientations[1] = GetYAxis(Matrix);
	obb_.orientations[2] = GetZAxis(Matrix);

}
