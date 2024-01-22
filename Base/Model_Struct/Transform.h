#pragma once
#include "Math_Structs.h"

struct Transform {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};
struct TransformQua {
	Vector3 scale;
	Quaternion quaternion;
	Vector3 translate;
};