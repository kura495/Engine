#pragma once
#include "PostProsess/renderTextrue/PostProsess.h"

class PPVinette : public PostProsess
{
public:
	void Update()override;

private:
	bool CheckBox = true;
};