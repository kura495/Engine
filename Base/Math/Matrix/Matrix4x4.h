#pragma once
#include "Math/Vector/Vector3.h"

class Matrix4x4 final {
public:
	static const int kMatrixNum = 4;
	float m[kMatrixNum][kMatrixNum];
//関数
public:
	static Vector3 GetScale(const Matrix4x4& m1);

	static Vector3 GetRotation(const Matrix4x4& m1);

	static inline Vector3 GetTransform(const Matrix4x4& m1){
		return { m1.m[3][0],m1.m[3][1],m1.m[3][2] };
	}


};
