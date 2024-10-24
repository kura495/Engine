#pragma once
//カメラの基本クラス//
#include "ViewProj/ViewProjection.h"

class Camera
{
public:
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	ViewProjection& GetViewProjection() { return viewProj; }

protected:

	ViewProjection viewProj;

};

