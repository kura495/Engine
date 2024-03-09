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
	friend inline Vector3 operator+(const Vector3& v1, const Vector3& v2) {
		Vector3 me;
		me.x = v1.x + v2.x;
		me.y = v1.y + v2.y;
		me.z = v1.z + v2.z;
		return me;
	}
	Vector3 operator-(Vector3 other) {
		Vector3 me;
		me.x = this->x - other.x;
		me.y = this->y - other.y;
		me.z = this->z - other.z;
		return me;
	}
	inline Vector3 operator==(const Vector3& v1) const{
		return { v1.x,v1.y,v1.z };
	}
	inline Vector3 operator+=(const Vector3& other){
		return *this = *this + other;
	}
	Vector3 operator*(float other) {
		Vector3 me;
		me.x = this->x * other;
		me.y = this->y * other;
		me.z = this->z * other;
		return me;
	}
public:
	static inline float Dot(const Vector3& v1, const Vector3& v2) {
		float result = (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
		return result;
	}
	//ベクトルを
	inline float Length() {
		float result{};
		result = sqrt(Dot({x,y,z},{x,y,z}));
		return result;
	}
	static inline float Length(const Vector3& v1) {
		float result{};
		result = sqrt(Dot({ v1.x,v1.y,v1.z},{ v1.x,v1.y,v1.z}));
		return result;
	}
	static inline Vector3 Cross(const Vector3& v1, const Vector3& v2) {
		Vector3 result;
		result.x = (v1.y * v2.z) - (v1.z * v2.y);
		result.y = (v1.z * v2.x) - (v1.x * v2.z);
		result.z = (v1.x * v2.y) - (v1.y * v2.x);
		return result;
	};
};

