#include"Camera.h"

void Camera::Initialize(int32_t kClientWidth, int32_t kClientHeight)
{
	kClientWidth_= kClientWidth;
	kClientHeight_= kClientHeight;

	worldMatrix = MakeAffineMatrix({ 1,1,1 },Vector3 { 0,0,0 }, { 0,0,0 });
	Matrix4x4 cameraMatrix = CreateIdentity4x4();
	Matrix4x4 Move = MakeTranslateMatrix(translation_);
	cameraMatrix = Multiply(cameraMatrix, Move);
	ViewMatrix = Inverse(cameraMatrix);
	ProjectionMatrix = MakePerspectiveFovMatrix(FOV, float(kClientWidth_) / float(kClientHeight_), 0.1f, 100.0f);
	Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix,Multiply(ViewMatrix, ProjectionMatrix));
	transformationMatrixData = worldViewProjectionMatrix;
	input = Input::GetInstance();
	
}
void Camera::Update()
{
#ifdef _DEBUG
	if (DebucCameraFlag) {
		DebugCameraMove();
	}
#endif // _DEBUG
	worldMatrix = MakeAffineMatrix({ 1,1,1 }, Vector3{ 0,0,0 }, { 0,0,0 });
	Matrix4x4 cameraMatrix = CreateIdentity4x4();
	Matrix4x4 rotate = MakeRotateMatrix(rotation_);
	cameraMatrix = Multiply(cameraMatrix, rotate);
	//rotate後で消す！！
	Matrix4x4 Move = MakeTranslateMatrix(translation_);
	cameraMatrix = Multiply(cameraMatrix, Move);
	ViewMatrix = Inverse(cameraMatrix);
	
	ProjectionMatrix = MakePerspectiveFovMatrix(FOV, float(kClientWidth_) / float(kClientHeight_), 0.1f, 100.0f);
	Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(ViewMatrix, ProjectionMatrix));
	transformationMatrixData = worldViewProjectionMatrix;
}
#ifdef _DEBUG
void Camera::DebugCameraMove()
{
#pragma region rotation

#pragma endregion 回転
#pragma region translation_
		if (input->TriggerKey(DIK_A)) {
			
		}
		else if (input->TriggerKey(DIK_D)) {
			
		}
		if (input->TriggerKey(DIK_W)) {
			
		}
		else if (input->TriggerKey(DIK_S)) {
			
		}
		if (input->TriggerKey(DIK_E)) {
			
		}
		else if (input->TriggerKey(DIK_Q)) {
			
		}
#pragma endregion 移動
}
#endif // DEBUG