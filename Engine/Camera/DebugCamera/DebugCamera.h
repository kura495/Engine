#pragma once

#include "Input/Input.h"
#include "Camera/Camera.h"
#include "Calc.h"

class DebugCamera : public Camera {
public:
	void Initialize()override;
	void Update()override;

private:
	Vector3 rotate_ = { 0.0f };
};