#pragma once
//天球クラス//
#include "Object/IObject.h"

class SkyDome : public IObject
{
public:
	void Init();
	void Update();
	void Draw();

private:
	void ImGui();
};
