#pragma once
#include "Math/Vector/Vector3.h"
#include "Math/Quaternion/Quaternion.h"

class Matrix4x4 final {
public:
	static const int kMatrixNum = 4;
	float m[kMatrixNum][kMatrixNum];
//関数
public:

	Vector3 GetScale();

	Quaternion GetRotation();

	inline Vector3 GetTransform(){
		Vector3 result;
		result = { m[3][0],m[3][1],m[3][2] };
		return result;
	}

	Matrix4x4 Transpose()
    {
        Matrix4x4 tmpm;
        for (int l = 0; l < 4; l++)
        {
            for (int c = 0; c < 4; c++)
            {
                tmpm.m[l][c] = m[c][l];
            }
        }
        return tmpm;
    }

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
