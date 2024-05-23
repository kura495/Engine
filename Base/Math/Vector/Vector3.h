#pragma once
#include<cmath>
#include "externals/nlohmann/json.hpp"
class Vector3 final {
public:
	float x;
	float y;
	float z;
	//jsonにマクロを利用して構造体を登録
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Vector3, x, y, z)
public:

#pragma region Oparator
	/*オペレーター*/
	/*add*/
	Vector3 operator+(const Vector3& input)const {
		return Vector3(this->x + input.x, this->y + input.y, this->z + input.z);
	}
	Vector3 operator+=(const Vector3& input) {
		*this = *this + input;
		return *this;
	}
	/*subtract*/
	Vector3 operator-(const Vector3& input)const {
		return Vector3(this->x - input.x, this->y - input.y, this->z - input.z);
	}
	Vector3 operator-=(const Vector3& input) {
		*this = *this - input;
		return *this;
	}
	/*Multiply*/
	Vector3 operator*(const Vector3& input)const {
		return Vector3(this->x * input.x, this->y * input.y, this->z * input.z);
	}
	Vector3 operator*=(const Vector3& input) {
		*this = *this * input;
		return *this;
	}
	/*withfloat*/
	/*add*/
	Vector3 operator+(const float& input)const {
		return Vector3(this->x + input, this->y + input, this->z + input);
	}
	Vector3 operator+=(const float& input) {
		*this = *this + input;
		return *this;
	}
	/*subtract*/
	Vector3 operator-(const float& input)const {
		return Vector3(this->x - input, this->y - input, this->z - input);
	}
	Vector3 operator-=(const float& input) {
		*this = *this - input;
		return *this;
	}
	/*Multiply*/
	Vector3 operator*(const float& input)const {
		return Vector3(this->x * input, this->y * input, this->z * input);
	}
	Vector3 operator*=(const float& input) {
		*this = *this * input;
		return *this;
	}
	Vector3 operator/(const float& input)const {
		return Vector3(this->x / input, this->y / input, this->z / input);
	}
	Vector3 operator/=(const float& input) {
		*this = *this / input;
		return *this;
	}
#pragma endregion 
public:
	//ベクトルの内積
	//return float
	static float Dot(const Vector3& input1, const Vector3& input2);
	//ベクトルの外積
	//return Vector3
	static Vector3 Cross(const Vector3& input1, const Vector3& input2);
	//ベクトルの長さを求める式
	//return float
	static float Length(const Vector3& input);
	//ベクトルの長さを求める式
	//return float
	float Length();
	//正規化
	//return Vector3
	static Vector3 Normalize(const Vector3& input);
	//正規化
	//return Vector3
	Vector3 Normalize();
	//線形補間
	//return Vector3
	static Vector3 VectorLerp(const Vector3& v1, const Vector3& v2, float t);
};

