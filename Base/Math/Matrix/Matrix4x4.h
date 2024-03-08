#pragma once

#include "Math/Vector/Vector3.h"

class Matrix4x4 final {
public:
	static const int kMatrixNum = 4;
	float m[kMatrixNum][kMatrixNum];
//関数
public:
	static Vector3 GetScale() {}

	static inline Vector3 GetRotation() {}

	static inline Vector3 GetTransform(const Matrix4x4& m1){
		return { m1.m[0][3],m1.m[1][3],m1.m[2][3] };
	}


};
