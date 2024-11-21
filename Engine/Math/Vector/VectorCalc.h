#pragma once
#include"Math_Structs.h"
#include<cmath>
#include<cassert>
namespace Math {
	Vector3 VectorTransform(const Vector3& vector, const Matrix4x4& matrix);
}