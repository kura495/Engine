#pragma once
#include "Math/Vector/Vector3.h"

class Matrix4x4 final {
public:
	static const int kMatrixNum = 4;
	float m[kMatrixNum][kMatrixNum];
//関数
public:

	Vector3 GetScale();

	static Vector3 GetRotation(const Matrix4x4& m1);

	inline Vector3 GetTransform(){
		return { m[3][0],m[3][1],m[3][2] };
	}


};
