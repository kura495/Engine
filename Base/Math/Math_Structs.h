#pragma once
#include "Math/Matrix/Matrix3x3.h"
#include "Math/Matrix/Matrix4x4.h"
#include "Math/Vector/Vector2.h"
#include "Math/Vector/Vector3.h"
#include "Math/Vector/Vector4.h"
#include "externals/nlohmann/json.hpp"
struct Quaternion {
	float x;
	float y;
	float z;
	float w;
	//jsonにマクロを利用して構造体を登録
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Quaternion,x,y,z,w)

	friend inline Quaternion operator+(const Quaternion q1, const Quaternion q2) {
		Quaternion result;
		result.x = q1.x + q2.x;
		result.y = q1.y + q2.y;
		result.z = q1.z + q2.z;
		result.w = q1.w + q2.z;
		return result;
	}
	inline Quaternion operator=(const Quaternion q1) {
		Quaternion result;
		result.x = q1.x;
		result.y = q1.y;
		result.z = q1.z;
		result.w = q1.w;
		return result;
	}
};