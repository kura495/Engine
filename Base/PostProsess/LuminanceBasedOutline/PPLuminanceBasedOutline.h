#pragma once
#pragma once
#include "Base/PostProsess/PostProsess.h"

class PPLuminanceBasedOutline : public PostProsess
{
public:
	void Update()override;

private:
	bool CheckBox = false;
};