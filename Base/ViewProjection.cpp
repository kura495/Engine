#include "ViewProjection.h"

void ViewProjection::Initialize(){
	CreateConstBuffer();
	Map();
	UpdateMatrix();
	TransferMatrix();
}

void ViewProjection::CreateConstBuffer(){
	constBuff_VS = DirectXCommon::GetInstance()->CreateBufferResource(sizeof(ConstBufferDataViewProjection));
	constBuff_PS = DirectXCommon::GetInstance()->CreateBufferResource(sizeof(ConstBufferCameraPos));
}

void ViewProjection::Map(){
	constBuff_VS.Get()->Map(0, nullptr, reinterpret_cast<void**>(&constMap_VS));
	constBuff_PS.Get()->Map(0, nullptr, reinterpret_cast<void**>(&constMap_PS));
}

void ViewProjection::UpdateMatrix(){
	UpdateViewMatrix();
	UpdateProjectionMatrix();
	TransferMatrix();
}

void ViewProjection::TransferMatrix(){
	constMap_VS->view = matView;
	constMap_VS->projection = matProjection;
	constMap_PS->worldPosition = translation_;
}

void ViewProjection::UpdateViewMatrix(){
	CameraMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, rotation_, translation_);
	matView = Inverse(CameraMatrix);
}

void ViewProjection::UpdateProjectionMatrix(){
	matProjection = MakePerspectiveFovMatrix(FOV, aspectRatio, nearZ, farZ);
}
