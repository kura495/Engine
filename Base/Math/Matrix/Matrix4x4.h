#pragma once
#include "Math/Vector/Vector3.h"
#include "Math/Quaternion/Quaternion.h"

class Matrix4x4 final {
public:
	static const int kMatrixNum = 4;
	float m[kMatrixNum][kMatrixNum];
//関数
public:

	Vector3 GetScale();

	Quaternion GetRotation();

	inline Vector3 GetTransform(){
		Vector3 result;
		result = { m[3][0],m[3][1],m[3][2] };
		return result;
	}

};
