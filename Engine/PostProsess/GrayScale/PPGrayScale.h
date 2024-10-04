#pragma once
#include "PostProsess/PostProsess.h"

class PPGrayScale : public PostProsess
{
public:
	void Update()override;

private:
	bool CheckBox = false;
};