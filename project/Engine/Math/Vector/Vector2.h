#pragma once
#include<cmath>
#include <algorithm>
namespace Math {
	class Vector2 final {
	public:
		float x;
		float y;

	public:
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
		/// <summary>
		/// 線形補間
		/// </summary>
		/// <param name="v1">ベクトル</param>
		/// <param name="v2">ベクトル</param>
		/// <param name="t">パラメーター</param>
		/// <returns>Vector2</returns>
		static Vector2 VectorLerp(const Vector2& v1, const Vector2& v2, float t);
	};
}