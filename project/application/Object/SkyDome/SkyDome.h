#pragma once
//天球クラス//
#include "Object/IObject.h"

class SkyDome{
public:
	void Init();
	void Update();
	void Draw();

private:
	void ImGui();
	Vector3 uvT;
	Vector3 color;
	std::vector<Model*> models_;
	WorldTransform world_;
};
