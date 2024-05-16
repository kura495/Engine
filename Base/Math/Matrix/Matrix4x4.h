#pragma once
#include "Math/Vector/Vector3.h"
#include "Math/Quaternion/Quaternion.h"


class Matrix4x4 final {
public:
	static const int kMatrixNum = 4;
	float m[kMatrixNum][kMatrixNum];
//関数
public:
	/*オペレーター*/
	/*add*/
	Matrix4x4 operator+(const Matrix4x4 &input){
		for (int i = 0; i < kMatrixNum; i++) {
			for (int j = 0; j < kMatrixNum; j++) {
				this->m[i][j] = this->m[i][j] + input.m[i][j];
			}
		}
		return *this;
	}
	Matrix4x4 operator+=(const Matrix4x4 &input) {

		*this = *this + input;

		return *this;
	}
	/*subtract*/
	Matrix4x4 operator-(const Matrix4x4 &input) {

		for (int i = 0; i < kMatrixNum; i++) {
			for (int j = 0; j < kMatrixNum; j++) {
				this->m[i][j] = this->m[i][j] - input.m[i][j];
			}
		}
		return *this;
	}
	Matrix4x4 operator-=(const Matrix4x4 &input) {

		*this = *this - input;

		return *this;
	}
	/*Multiply*/
	Matrix4x4 operator*(const Matrix4x4 &input) {
		Matrix4x4 result;
		for (int i = 0; i < kMatrixNum; i++) {
			for (int j = 0; j < kMatrixNum; j++) {
				result.m[i][j] = this->m[i][0] * input.m[0][j] + this->m[i][1] * input.m[1][j] + this->m[i][2] * input.m[2][j] + this->m[i][3] * input.m[3][j];
			}
		}

		return result;
	}
	Matrix4x4 operator*=(const Matrix4x4 &input) {

		*this = *this * input;

		return *this;
	}

	//行列からScaleの値を抜き出す
	//return : Vector3
	Vector3 GetScale();

	//行列からRotateの値を抜き出す
	//return : Quaternion
	Quaternion GetRotation();

	//行列からTranslateの値を抜き出す
	//return : Vector3
	inline Vector3 GetTransform(){
		Vector3 result;
		result = { m[3][0],m[3][1],m[3][2] };
		return result;
	}

	//行列を反転 転置行列をつくる
	//return : Matrix4x4
	Matrix4x4 Transpose();

	static float det

	/*Matrix4x4 Inverse()
	{
		Matrix4x4 result;
		float resultDet = det(*this);
		result.m[0][0] = ((*this).m[1][1] * (*this).m[2][2] * (*this).m[3][3] + (*this).m[1][2] * (*this).m[2][3] * (*this).m[3][1] + (*this).m[1][3] * (*this).m[2][1] * (*this).m[3][2]
			- (*this).m[1][3] * (*this).m[2][2] * (*this).m[3][1] - (*this).m[1][2] * (*this).m[2][1] * (*this).m[3][3] - (*this).m[1][1] * (*this).m[2][3] * (*this).m[3][2]) / resultDet;
		result.m[0][1] = (-(*this).m[0][1] * (*this).m[2][2] * (*this).m[3][3] - (*this).m[0][2] * (*this).m[2][3] * (*this).m[3][1] - (*this).m[0][3] * (*this).m[2][1] * (*this).m[3][2]
			+ (*this).m[0][3] * (*this).m[2][2] * (*this).m[3][1] + (*this).m[0][2] * (*this).m[2][1] * (*this).m[3][3] + (*this).m[0][1] * (*this).m[2][3] * (*this).m[3][2]) / resultDet;
		result.m[0][2] = ((*this).m[0][1] * (*this).m[1][2] * (*this).m[3][3] + (*this).m[0][2] * (*this).m[1][3] * (*this).m[3][1] + (*this).m[0][3] * (*this).m[1][1] * (*this).m[3][2]
			- (*this).m[0][3] * (*this).m[1][2] * (*this).m[3][1] - (*this).m[0][2] * (*this).m[1][1] * (*this).m[3][3] - (*this).m[0][1] * (*this).m[1][3] * (*this).m[3][2]) / resultDet;
		result.m[0][3] = (-(*this).m[0][1] * (*this).m[1][2] * (*this).m[2][3] - (*this).m[0][2] * (*this).m[1][3] * (*this).m[2][1] - (*this).m[0][3] * (*this).m[1][1] * (*this).m[2][2]
			+ (*this).m[0][3] * (*this).m[1][2] * (*this).m[2][1] + (*this).m[0][2] * (*this).m[1][1] * (*this).m[2][3] + (*this).m[0][1] * (*this).m[1][3] * (*this).m[2][2]) / resultDet;

		result.m[1][0] = (-(*this).m[1][0] * (*this).m[2][2] * (*this).m[3][3] - (*this).m[1][2] * (*this).m[2][3] * (*this).m[3][0] - (*this).m[1][3] * (*this).m[2][0] * (*this).m[3][2]
			+ (*this).m[1][3] * (*this).m[2][2] * (*this).m[3][0] + (*this).m[1][2] * (*this).m[2][0] * (*this).m[3][3] + (*this).m[1][0] * (*this).m[2][3] * (*this).m[3][2]) / resultDet;
		result.m[1][1] = ((*this).m[0][0] * (*this).m[2][2] * (*this).m[3][3] + (*this).m[0][2] * (*this).m[2][3] * (*this).m[3][0] + (*this).m[0][3] * (*this).m[2][0] * (*this).m[3][2]
			- (*this).m[0][3] * (*this).m[2][2] * (*this).m[3][0] - (*this).m[0][2] * (*this).m[2][0] * (*this).m[3][3] - (*this).m[0][0] * (*this).m[2][3] * (*this).m[3][2]) / resultDet;
		result.m[1][2] = (-(*this).m[0][0] * (*this).m[1][2] * (*this).m[3][3] - (*this).m[0][2] * (*this).m[1][3] * (*this).m[3][0] - (*this).m[0][3] * (*this).m[1][0] * (*this).m[3][2]
			+ (*this).m[0][3] * (*this).m[1][2] * (*this).m[3][0] + (*this).m[0][2] * (*this).m[1][0] * (*this).m[3][3] + (*this).m[0][0] * (*this).m[1][3] * (*this).m[3][2]) / resultDet;
		result.m[1][3] = ((*this).m[0][0] * (*this).m[1][2] * (*this).m[2][3] + (*this).m[0][2] * (*this).m[1][3] * (*this).m[2][0] + (*this).m[0][3] * (*this).m[1][0] * (*this).m[2][2]
			- (*this).m[0][3] * (*this).m[1][2] * (*this).m[2][0] - (*this).m[0][2] * (*this).m[1][0] * (*this).m[2][3] - (*this).m[0][0] * (*this).m[1][3] * (*this).m[2][2]) / resultDet;

		result.m[2][0] = ((*this).m[1][0] * (*this).m[2][1] * (*this).m[3][3] + (*this).m[1][1] * (*this).m[2][3] * (*this).m[3][0] + (*this).m[1][3] * (*this).m[2][0] * (*this).m[3][1]
			- (*this).m[1][3] * (*this).m[2][1] * (*this).m[3][0] - (*this).m[1][1] * (*this).m[2][0] * (*this).m[3][3] - (*this).m[1][0] * (*this).m[2][3] * (*this).m[3][1]) / resultDet;
		result.m[2][1] = (-(*this).m[0][0] * (*this).m[2][1] * (*this).m[3][3] - (*this).m[0][1] * (*this).m[2][3] * (*this).m[3][0] - (*this).m[0][3] * (*this).m[2][0] * (*this).m[3][1]
			+ (*this).m[0][3] * (*this).m[2][1] * (*this).m[3][0] + (*this).m[0][1] * (*this).m[2][0] * (*this).m[3][3] + (*this).m[0][0] * (*this).m[2][3] * (*this).m[3][1]) / resultDet;
		result.m[2][2] = ((*this).m[0][0] * (*this).m[1][1] * (*this).m[3][3] + (*this).m[0][1] * (*this).m[1][3] * (*this).m[3][0] + (*this).m[0][3] * (*this).m[1][0] * (*this).m[3][1]
			- (*this).m[0][3] * (*this).m[1][1] * (*this).m[3][0] - (*this).m[0][1] * (*this).m[1][0] * (*this).m[3][3] - (*this).m[0][0] * (*this).m[1][3] * (*this).m[3][1]) / resultDet;
		result.m[2][3] = (-(*this).m[0][0] * (*this).m[1][1] * (*this).m[2][3] - (*this).m[0][1] * (*this).m[1][3] * (*this).m[2][0] - (*this).m[0][3] * (*this).m[1][0] * (*this).m[2][1]
			+ (*this).m[0][3] * (*this).m[1][1] * (*this).m[2][0] + (*this).m[0][1] * (*this).m[1][0] * (*this).m[2][3] + (*this).m[0][0] * (*this).m[1][3] * (*this).m[2][1]) / resultDet;

		result.m[3][0] = (-(*this).m[1][0] * (*this).m[2][1] * (*this).m[3][2] - (*this).m[1][1] * (*this).m[2][2] * (*this).m[3][0] - (*this).m[1][2] * (*this).m[2][0] * (*this).m[3][1]
			+ (*this).m[1][2] * (*this).m[2][1] * (*this).m[3][0] + (*this).m[1][1] * (*this).m[2][0] * (*this).m[3][2] + (*this).m[1][0] * (*this).m[2][2] * (*this).m[3][1]) / resultDet;
		result.m[3][1] = ((*this).m[0][0] * (*this).m[2][1] * (*this).m[3][2] + (*this).m[0][1] * (*this).m[2][2] * (*this).m[3][0] + (*this).m[0][2] * (*this).m[2][0] * (*this).m[3][1]
			- (*this).m[0][2] * (*this).m[2][1] * (*this).m[3][0] - (*this).m[0][1] * (*this).m[2][0] * (*this).m[3][2] - (*this).m[0][0] * (*this).m[2][2] * (*this).m[3][1]) / resultDet;
		result.m[3][2] = (-(*this).m[0][0] * (*this).m[1][1] * (*this).m[3][2] - (*this).m[0][1] * (*this).m[1][2] * (*this).m[3][0] - (*this).m[0][2] * (*this).m[1][0] * (*this).m[3][1]
			+ (*this).m[0][2] * (*this).m[1][1] * (*this).m[3][0] + (*this).m[0][1] * (*this).m[1][0] * (*this).m[3][2] + (*this).m[0][0] * (*this).m[1][2] * (*this).m[3][1]) / resultDet;
		result.m[3][3] = ((*this).m[0][0] * (*this).m[1][1] * (*this).m[2][2] + (*this).m[0][1] * (*this).m[1][2] * (*this).m[2][0] + (*this).m[0][2] * (*this).m[1][0] * (*this).m[2][1]
			- (*this).m[0][2] * (*this).m[1][1] * (*this).m[2][0] - (*this).m[0][1] * (*this).m[1][0] * (*this).m[2][2] - (*this).m[0][0] * (*this).m[1][2] * (*this).m[2][1]) / resultDet;
		return result;
	}*/
};
