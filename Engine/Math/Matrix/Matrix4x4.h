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
		//[0][0],[1][1],[2][2],[3][3]に1を代入、それ以外に0を代入した行列を作成
		//return : Matrix4x4
		static Matrix4x4 CreateIdentity();

		static Matrix4x4 Multiply(const Matrix4x4& input1, const Matrix4x4& input2);
		//行列からScaleの値を抜き出す
		//return : Vector3
		Vector3 GetScale();

		//行列からRotateの値を抜き出す
		//return : Quaternion
		Quaternion GetRotation();

		//行列からTranslateの値を抜き出す
		//return : Vector3
		inline Vector3 GetTransform() {
			Vector3 result;
			result = { m[3][0],m[3][1],m[3][2] };
			return result;
		}

		//行列を反転 転置行列をつくる
		//return : Matrix4x4
		static Matrix4x4 Transpose(const Matrix4x4& m);
		//行列を反転 転置行列をつくる
		//return : Matrix4x4
		Matrix4x4 Transpose();
		//行列式の計算
		//return : float
		static float det(const Matrix4x4& m);
		//行列式の計算
		//return : float
		float det();
		//逆行列の計算
		//return : Matrix4x4
		static Matrix4x4 Inverse(const Matrix4x4& m);
		//逆行列の計算
		//return : Matrix4x4
		Matrix4x4 Inverse();

	};
}