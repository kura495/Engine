#include "WorldTransform.h"
#include "Base/Object/Model/Model.h"

void WorldTransform::Initialize()
{
	matWorld_ = Matrix4x4::CreateIdentity();
	CreateConstBuffer();
	Map();
	TransferMatrix();
}

void WorldTransform::CreateConstBuffer()
{
	constBuff_ = DirectXCommon::GetInstance()->CreateBufferResource(sizeof(ConstBufferDataWorldTransform));
}

void WorldTransform::Map()
{
	constBuff_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&constMap));
}

void WorldTransform::TransferMatrix()
{
	constMap->matWorld = matWorld_;
	constMap->WorldInverseTranspose = Matrix4x4::Transpose(Matrix4x4::Inverse(matWorld_)); 
}

void WorldTransform::UpdateMatrix()
{
	Matrix4x4 AffineMatrix = MakeAffineMatrix(transform_.scale, transform_.quaternion, transform_.translate);
	matWorld_ = AffineMatrix;
	//親があれば親のワールド行列を掛ける
	if (parent_) {
		matWorld_ = Matrix4x4::Multiply(matWorld_, parent_->matWorld_);
	}

	TransferMatrix();
}

Vector3 WorldTransform::GetTranslateFromMatWorld()const
{
	Vector3 worldPos;
	worldPos.x = matWorld_.m[3][0];
	worldPos.y = matWorld_.m[3][1];
	worldPos.z = matWorld_.m[3][2];
	return worldPos;
}

void WorldTransform::SetTransform(Model* model) {
	transform_.translate += model->GetModelData().rootNode.localMatrix.GetTransform();
	if (model->GetModelData().rootNode.localMatrix.GetScale().x != 1.0f &&
		model->GetModelData().rootNode.localMatrix.GetScale().y != 1.0f &&
		model->GetModelData().rootNode.localMatrix.GetScale().z != 1.0f
		) {
		transform_.scale += model->GetModelData().rootNode.localMatrix.GetScale();
	}

	transform_.quaternion = Quaternion::Normalize(model->GetModelData().rootNode.localMatrix.GetRotation());
	UpdateMatrix();
};