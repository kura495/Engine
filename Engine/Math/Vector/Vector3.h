#pragma once
#include<cmath>
#include "externals/nlohmann/json.hpp"

namespace Math {
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
		/// <summary>
		/// ベクトルの内積
		/// </summary>
		/// <param name="input1">ベクトル</param>
		/// <param name="input2">ベクトル</param>
		/// <returns>float</returns>
		static float Dot(const Vector3& input1, const Vector3& input2);
		/// <summary>
		/// ベクトルの外積
		/// </summary>
		/// <param name="input1">ベクトル</param>
		/// <param name="input2">ベクトル</param>
		/// <returns>Vector3</returns>
		static Vector3 Cross(const Vector3& input1, const Vector3& input2);
		/// <summary>
		/// ベクトルの長さを求める式
		/// </summary>
		/// <param name="input">ベクトル</param>
		/// <returns>float</returns>
		static float Length(const Vector3& input);
		/// <summary>
		/// ベクトルの長さを求める式
		/// </summary>
		/// <returns>float</returns>
		float Length();
		/// <summary>
		/// ベクトルの距離を求める式
		/// </summary>
		/// <param name="input1">スタート地点</param>
		/// <param name="input2">ゴール地点</param>
		/// <returns>スタート地点からゴール地点までの距離</returns>
		static float Distance(const Vector3& input1, const Vector3& input2);
		/// <summary>
		/// 正規化
		/// </summary>
		/// <param name="input">ベクトル</param>
		/// <returns>Vector3</returns>
		static Vector3 Normalize(const Vector3& input);
		/// <summary>
		/// 正規化
		/// </summary>
		/// <returns>Vector3</returns>
		Vector3 Normalize();
		/// <summary>
		/// 線形補間
		/// </summary>
		/// <param name="v1">ベクトル</param>
		/// <param name="v2">ベクトル</param>
		/// <param name="t">パラメーター</param>
		/// <returns>Vector3</returns>
		static Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);
	};
}
