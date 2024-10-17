#pragma once
#include "Math/Vector/Vector3.h"
#include "DirectXMath.h"
#include <numbers>

class Quaternion final {
public:
	float x;
	float y;
	float z;
	float w;
	//jsonにマクロを利用して構造体を登録
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Quaternion, x, y, z, w)
public:
#pragma region Oparator
	Quaternion operator+(const Quaternion input)const {
		 return Quaternion(this->x + input.x, this->y + input.y, this->z + input.z, this->w + input.w);
	}
	Quaternion operator+=(const Quaternion input) {
		*this = *this + input;
		return *this;
	}
	Quaternion operator-(const Quaternion input)const {
		 return Quaternion(this->x - input.x, this->y - input.y, this->z - input.z, this->w - input.w);
	}
	Quaternion operator-=(const Quaternion input) {
		*this = *this - input;
		return *this;
	}
	Quaternion operator*(const Quaternion input)const {
		Quaternion result;
		Vector3 cross = Vector3::Cross({ this->x,this->y,this->z }, { input.x,input.y,input.z });
		float dot = Vector3::Dot({ this->x,this->y,this->z }, { input.x,input.y,input.z });
		result.x = cross.x + input.w * this->x + this->w * input.x;
		result.y = cross.y + input.w * this->y + this->w * input.y;
		result.z = cross.z + input.w * this->z + this->w * input.z;
		result.w = this->w * input.w - dot;

		return result;
	}
	Quaternion operator*=(const Quaternion input) {
		*this = *this * input;
		return *this;
	}
#pragma endregion
public:
	//単位Quaternionを作る
	//return Quaternion
	static Quaternion IdentityQuaternion();
	//共役Quaternionを返す
	//return Quaternion
	static Quaternion Conjugate(const Quaternion& quaternion);
	//共役Quaternionを返す
	//return Quaternion
	Quaternion Conjugate();
	//Quaternionのnormを返す
	//return float
	static float Norm(const Quaternion& quaternion);
	//Quaternionのnormを返す
	//return float
	float Norm();
	//逆Quaternionを返す
	//return Quaternion
	static Quaternion Inverse(const Quaternion& quaternion);
	//逆Quaternionを返す
	//return Quaternion
	Quaternion Inverse();
	//正規化したQuaternionを返す
	//return Quaternion
	static Quaternion Normalize(const Quaternion& quaternion);
	//正規化したQuaternionを返す
	//return Quaternion
	Quaternion Normalize();
	//球面線形補間
	//return Quaternion
	static Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, float t);
	//オイラー角をクォータニオン
	//return Quaternion
	static Quaternion EulerToQuaterion(Vector3 input);
	
	//任意軸回転を表すQuaternionの生成
	static Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle);
};




