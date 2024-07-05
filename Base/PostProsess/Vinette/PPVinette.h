#pragma once
#include "Base/PostProsess/PostProsess.h"

class PPVinette : public PostProsess
{
public:
	void Update()override;

private:
	bool CheckBox = true;
};