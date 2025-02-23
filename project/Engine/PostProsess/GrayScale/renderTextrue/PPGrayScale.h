#pragma once
#include "PostProsess/renderTextrue/PostProsess.h"

class PPGrayScale : public PostProsess
{
public:
	void Update()override;

private:
	bool CheckBox = false;
};