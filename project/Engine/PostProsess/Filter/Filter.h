#pragma once
#include "PostProsess/PostProsess.h"

class PPFilter : public PostProsess
{
public:
	void Update()override;

private:
	bool CheckBox = false;
};