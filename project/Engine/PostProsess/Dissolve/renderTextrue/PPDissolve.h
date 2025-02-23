#pragma once
#include "PostProsess/renderTextrue/PostProsess.h"

class PPDissolve : public PostProsess
{
public:
	void Update()override;

private:
	bool CheckBox = false;
};