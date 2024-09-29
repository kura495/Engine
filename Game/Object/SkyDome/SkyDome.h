#pragma once
#include "Base/Object/IObject.h"

class SkyDome : public IObject
{
public:
	void Init();
	void Update();
	void Draw();

private:
	void ImGui();
};
