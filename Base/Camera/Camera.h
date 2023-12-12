#pragma once
#include "MatrixCalc.h"
#include "VectorCalc.h"
#include "Transform.h"
#include "ViewProjection.h"
#include "Math_Structs.h"
#include "ImGuiManager.h"
#include "Input.h"

class Camera
{
public:
	void Initialize();
	void Update();
	const ViewProjection& GetViewProjection() { viewProj.UpdateMatrix();return viewProj; }

#ifdef _DEBUG
	void DebugCamera(bool Flag) {
		DebucCameraFlag = Flag;
}
#endif // DEBUG

private:
	Input* input = nullptr;
	//FOV
	float FOV = 45.0f;

	ViewProjection viewProj;
	
#ifdef _DEBUG
	bool DebucCameraFlag = false;
	void DebugCameraMove();
#endif // DEBUG

};

