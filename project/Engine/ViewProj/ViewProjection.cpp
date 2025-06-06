#include "ViewProjection.h"

void ViewProjection::Initialize(){
	CreateConstBuffer();
	Map();
	Update();
}

void ViewProjection::CreateConstBuffer(){
	constBuff_VS = DirectXCommon::GetInstance()->CreateBufferResource(sizeof(ConstBufferDataViewProjection));
	constBuff_PS = DirectXCommon::GetInstance()->CreateBufferResource(sizeof(ConstBufferCameraPos));
}

void ViewProjection::Map(){
	constBuff_VS.Get()->Map(0, nullptr, reinterpret_cast<void**>(&constMap_VS));
	constBuff_PS.Get()->Map(0, nullptr, reinterpret_cast<void**>(&constMap_PS));
}

void ViewProjection::Update(){
	UpdateViewMatrix();
	UpdateProjectionMatrix();
	TransferMatrix();
}

void ViewProjection::TransferMatrix(){
	constMap_VS->view = matView;
	constMap_VS->projection = matProjection;
	constMap_PS->worldPosition = parameter.translation_;
}

void ViewProjection::UpdateViewMatrix(){
	parameter.rotation_ = parameter.rotation_.Normalize();
	CameraMatrix = MakeAffineMatrix(Vector3{ 1.0f,1.0f,1.0f }, parameter.rotation_, parameter.translation_);
	matView = Matrix4x4::Inverse(CameraMatrix);
}

void ViewProjection::UpdateProjectionMatrix(){
	matProjection = MakePerspectiveFovMatrix(parameter.FOV, aspectRatio, nearZ, farZ);
}
