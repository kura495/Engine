#pragma once
#include<cmath>
class Vector3 final{
public:
	float x;
	float y;
	float z;
public:
	Vector3 operator +(Vector3 other) {
		Vector3 me;
		me.x = this->x + other.x;
		me.y = this->y + other.y;
		me.z = this->z + other.z;
		return me;
	}
	Vector3 operator -(Vector3 other) {
		Vector3 me;
		me.x = this->x - other.x;
		me.y = this->y - other.y;
		me.z = this->z - other.z;
		return me;
	}
	Vector3 operator =(Vector3* other) {
		Vector3 me;
		me.x = other->x;
		me.y = other->y;
		me.z = other->z;
		return me;
	}
	Vector3 operator +=(Vector3 other) {
		this->x = this->x + other.x;
		this->y = this->y + other.y;
		this->z = this->z + other.z;
		return { this->x,this->y,this->z };
	}
	Vector3 operator *(float other) {
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
};

