#pragma once

#include "Base/Input/Input.h"
#include "Base/Camera/Camera.h"

class DebugCamera : public Camera {
public:
	void Initialize()override;
	void Update()override;
	void ImGui();

private:

};