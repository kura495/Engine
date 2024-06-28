#include "OBBoxCollider.h"

void OBBoxCollider::Initalize()
{
}

void OBBoxCollider::CollisionUpdate()
{
	obb_.center = center_->transform_.translate;
	obb_.size[0] = size_.x;
	obb_.size[1] = size_.y;
	obb_.size[2] = size_.z;
	SetOrientations(MakeRotateMatrix(center_->transform_.quaternion));
}

void OBBoxCollider::CollisionDraw()
{
}

void OBBoxCollider::SetOrientations(Matrix4x4 Matrix)
{
	//X成分
	obb_.orientations[0] = GetXAxis(Matrix);
	obb_.orientations[1] = GetYAxis(Matrix);
	obb_.orientations[2] = GetZAxis(Matrix);

}
