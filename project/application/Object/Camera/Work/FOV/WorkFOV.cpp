#include "WorkFOV.h"

void WorkFOV::Init(float FOVvalue)
{
	easeT = 0.0f;
	flag = true;
	newFOVvalue = FOVvalue;
}

void WorkFOV::Update(float& FOV)
{
	if (flag) {
		easeT = (std::min)(easeT + AddEaseT, 1.0f);
		FOV = Lerp(FOV, newFOVvalue, easeT);
		if (easeT == 1.0f) {
			flag = false;
		}
	}
}
