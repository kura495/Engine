#pragma once
//カメラの基本クラス//
#include "ViewProj/ViewProjection.h"

class Camera
{
public:
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	CameraParameter GetParameter() const { return parameter; }
protected:
	CameraParameter parameter;
};

