#pragma once
#include "Math/Vector/Vector3.h"
class Quaternion final {
public:
	float x;
	float y;
	float z;
	float w;
	//jsonにマクロを利用して構造体を登録
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Quaternion, x, y, z, w)
public:
	friend inline Quaternion operator+(const Quaternion q1, const Quaternion q2) {
		Quaternion result;
		result.x = q1.x + q2.x;
		result.y = q1.y + q2.y;
		result.z = q1.z + q2.z;
		result.w = q1.w + q2.z;
		return result;
	}
	Quaternion operator+=(const Quaternion q1) {
		this->x = q1.x;
		this->y = q1.y;
		this->z = q1.z;
		this->w = q1.w;
		return *this;
	}
	inline Quaternion operator==(const Quaternion q1) const {
		Quaternion result;
		result.x = q1.x;
		result.y = q1.y;
		result.z = q1.z;
		result.w = q1.w;
		return result;
	}
public:

};
//Quaternionの積
Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs);
//単位Quaternionを返す
Quaternion IdentityQuaternion();
//共役Quaternionを返す
Quaternion Conjugate(const Quaternion& quaternion);
//Quaternionのnormを返す
float Norm(const Quaternion& quaternion);
//逆Quaternionを返す
Quaternion Inverse(const Quaternion& quaternion);
//任意軸回転を表すQuaternionの生成
Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle);
Quaternion Lerp(const Quaternion& q0, const Quaternion& q1, float t);
Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, float t);
//正規化したQuaternionを返す
Quaternion Normalize(const Quaternion& quaternion);
