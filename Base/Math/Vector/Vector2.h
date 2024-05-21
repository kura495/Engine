#pragma once
struct Vector2 final {
	float x;
	float y;

#pragma region Oparator
	/*オペレーター*/
	/*add*/
	Vector2 operator+(const Vector2& input) {
		this->x = this->x + input.x;
		this->y = this->y + input.y;
		return *this;
	}
	Vector2 operator+=(const Vector2& input) {

		*this = *this + input;

		return *this;
	}
	/*subtract*/
	Vector2 operator-(const Vector2& input) {
		this->x = this->x - input.x;
		this->y = this->y - input.y;
		return *this;
	}
	Vector2 operator-=(const Vector2& input) {

		*this = *this - input;

		return *this;
	}
	/*Multiply*/
	Vector2 operator*(const Vector2& input) {
		this->x = this->x * input.x;
		this->y = this->y * input.y;

		return *this;
	}
	Vector2 operator*=(const Vector2& input) {

		*this = *this * input;

		return *this;
	}
	/*withfloat*/
	/*add*/
	Vector2 operator+(const float& input) {

		return *this;
	}
	Vector2 operator+=(const float& input) {

		*this = *this + input;

		return *this;
	}
	/*subtract*/
	Vector2 operator-(const float& input) {

		return *this;
	}
	Vector2 operator-=(const float& input) {

		*this = *this - input;

		return *this;
	}
	/*Multiply*/
	Vector2 operator*(const float& input) {

		return *this;
	}
	Vector2 operator*=(const Vector2& input) {

		*this = *this * input;

		return *this;
	}
#pragma endregion 
};

// TODO : コピー用
//#pragma region Oparator
///*オペレーター*/
///*add*/
//Vector2 operator+(const Vector2& input) {
//
//	return *this;
//}
//Vector2 operator+=(const Vector2& input) {
//
//	*this = *this + input;
//
//	return *this;
//}
///*subtract*/
//Vector2 operator-(const Vector2& input) {
//
//	return *this;
//}
//Vector2 operator-=(const Vector2& input) {
//
//	*this = *this - input;
//
//	return *this;
//}
///*Multiply*/
//Vector2 operator*(const Vector2& input) {
//	Vector2 result;
//
//	return result;
//}
//Vector2 operator*=(const Vector2& input) {
//
//	*this = *this * input;
//
//	return *this;
//}
//
//#pragma endregion 