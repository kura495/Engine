#pragma once
#pragma once
#include "PostProsess/PostProsess.h"

class PPLuminanceBasedOutline : public PostProsess
{
public:
	void Update()override;

private:
	bool CheckBox = false;
};