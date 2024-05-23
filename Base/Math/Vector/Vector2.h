#pragma once
#include<cmath>
struct Vector2 final {
	float x;
	float y;

#pragma region Oparator
	/*オペレーター*/
	/*add*/
	Vector2 operator+(const Vector2& input)const {
		return Vector2(this->x + input.x, this->y + input.y);
	}
	Vector2 operator+=(const Vector2& input) {

		*this = *this + input;

		return *this;
	}
	/*subtract*/
	Vector2 operator-(const Vector2& input)const {
		return Vector2(this->x - input.x, this->y - input.y);
	}
	Vector2 operator-=(const Vector2& input) {
		*this = *this - input;
		return *this;
	}
	/*Multiply*/
	Vector2 operator*(const Vector2& input)const {
		return Vector2(this->x * input.x, this->y * input.y);
	}
	Vector2 operator*=(const Vector2& input) {

		*this = *this * input;

		return *this;
	}
	/*withfloat*/
	/*add*/
	Vector2 operator+(const float& input)const {
		return Vector2(this->x + input, this->y + input);
	}
	Vector2 operator+=(const float& input) {

		*this = *this + input;

		return *this;
	}
	/*subtract*/
	Vector2 operator-(const float& input)const {
		return Vector2(this->x - input, this->y - input);
	}
	Vector2 operator-=(const float& input) {

		*this = *this - input;

		return *this;
	}
	/*Multiply*/
	Vector2 operator*(const float& input)const {
		return Vector2(this->x * input, this->y * input);
	}
	Vector2 operator*=(const float& input) {

		*this = *this * input;

		return *this;
	}
#pragma endregion 

public:
	//線形補間
	//return Vector2
	static Vector2 VectorLerp(const Vector2& v1, const Vector2& v2, float t);
};