#pragma once
#include "Math/Vector/Vector3.h"
#include "Math/Quaternion/Quaternion.h"

namespace Math {
	class Matrix4x4 final {
	public:
		static const int kMatrixNum = 4;
		float m[kMatrixNum][kMatrixNum];
		//関数
	public:

#pragma region Oparator
		/*オペレーター*/
		/*add*/
		Matrix4x4 operator+(const Matrix4x4& input) {
			Matrix4x4 result;
			for (int i = 0; i < kMatrixNum; i++) {
				for (int j = 0; j < kMatrixNum; j++) {
					result.m[i][j] = this->m[i][j] + input.m[i][j];
				}
			}
			return result;
		}
		Matrix4x4 operator+=(const Matrix4x4& input) {

			*this = *this + input;

			return *this;
		}
		/*subtract*/
		Matrix4x4 operator-(const Matrix4x4& input) {
			Matrix4x4 result;
			for (int i = 0; i < kMatrixNum; i++) {
				for (int j = 0; j < kMatrixNum; j++) {
					result.m[i][j] = this->m[i][j] - input.m[i][j];
				}
			}
			return result;
		}
		Matrix4x4 operator-=(const Matrix4x4& input) {

			*this = *this - input;

			return *this;
		}
		/*Multiply*/
		Matrix4x4 operator*(const Matrix4x4& input) {
			Matrix4x4 result;
			for (int i = 0; i < kMatrixNum; i++) {
				for (int j = 0; j < kMatrixNum; j++) {
					result.m[i][j] = this->m[i][0] * input.m[0][j] + this->m[i][1] * input.m[1][j] + this->m[i][2] * input.m[2][j] + this->m[i][3] * input.m[3][j];
				}
			}
			return result;
		}
		Matrix4x4 operator*=(const Matrix4x4& input) {

			*this = *this * input;

			return *this;
		}
#pragma endregion 
		/// <summary>
		/// [0][0],[1][1],[2][2],[3][3]に1を代入、それ以外に0を代入した行列を作成
		/// </summary>
		/// <returns>Matrix4x4</returns>
		static Matrix4x4 CreateIdentity();
		/// <summary>
		/// 乗算
		/// </summary>
		/// <param name="input1">4x4行列</param>
		/// <param name="input2">4x4行列</param>
		/// <returns>Matrix4x4</returns>
		static Matrix4x4 Multiply(const Matrix4x4& input1, const Matrix4x4& input2);
		/// <summary>
		/// 行列からScaleの値を抜き出す
		/// </summary>
		/// <returns>Vector3</returns>
		Vector3 GetScale();
		/// <summary>
		/// 行列からRotateの値を抜き出す
		/// </summary>
		/// <returns>Quaternion</returns>
		Quaternion GetRotation();
		/// <summary>
		/// 行列からTranslateの値を抜き出す
		/// </summary>
		/// <returns>Vector3</returns>
		inline Vector3 GetTransform() {
			Vector3 result;
			result = { m[3][0],m[3][1],m[3][2] };
			return result;
		}
		/// <summary>
		/// 行列を反転 転置行列をつくる
		/// </summary>
		/// <param name="m">4x4行列</param>
		/// <returns>Matrix4x4</returns>
		static Matrix4x4 Transpose(const Matrix4x4& m);
		/// <summary>
		/// 行列を反転 転置行列をつくる
		/// </summary>
		/// <returns>Matrix4x4</returns>
		Matrix4x4 Transpose();
		/// <summary>
		/// 行列式の計算
		/// </summary>
		/// <param name="m">4x4行列</param>
		/// <returns>float</returns>
		static float det(const Matrix4x4& m);
		/// <summary>
		/// 行列式の計算
		/// </summary>
		/// <returns>float</returns>
		float det();
		/// <summary>
		/// 逆行列の計算
		/// </summary>
		/// <param name="m">4x4行列</param>
		/// <returns>Matrix4x4</returns>
		static Matrix4x4 Inverse(const Matrix4x4& m);
		/// <summary>
		/// 逆行列の計算
		/// </summary>
		/// <returns>Matrix4x4</returns>
		Matrix4x4 Inverse();

	};
}