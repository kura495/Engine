#include "Matrix4x4.h"
using namespace Math;
Matrix4x4 Matrix4x4::CreateIdentity()
{
    Matrix4x4 result = {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f,
    };
    return result;
}

Matrix4x4 Matrix4x4::Multiply(const Matrix4x4& input1, const Matrix4x4& input2)
{
    Matrix4x4 result;
    for (int i = 0; i < kMatrixNum; i++) {
        for (int j = 0; j < kMatrixNum; j++) {
            result.m[i][j] = input1.m[i][0] * input2.m[0][j] + input1.m[i][1] * input2.m[1][j] + input1.m[i][2] * input2.m[2][j] + input1.m[i][3] * input2.m[3][j];
        }
    }
    return result;
}

Vector3 Matrix4x4::GetScale() {
	Vector3 result;
	result = {
		Vector3::Length({m[0][0],m[0][1],m[0][2]}),
		Vector3::Length({m[1][0],m[1][1],m[1][2]}),
		Vector3::Length({m[2][0],m[2][1],m[2][2]})
	};
	return result;
}

Quaternion Matrix4x4::GetRotation() {
    float px = m[0][0] - m[1][1] - m[2][2] + 1;
    float py = -m[0][0] + m[1][1] - m[2][2] + 1;
    float pz = -m[0][0] - m[1][1] + m[2][2] + 1;
    float pw = m[0][0] + m[1][1] + m[2][2] + 1;

    int selected = 0;
    float max = px;
    if (max < py) {
        selected = 1;
        max = py;
    }
    if (max < pz) {
        selected = 2;
        max = pz;
    }
    if (max < pw) {
        selected = 3;
        max = pw;
    }

    if (selected == 0) {
        float x = std::sqrt(px) * 0.5f;
        float d = 1 / (4 * x);
        return Quaternion(
            x,
            (m[1][0] + m[0][1]) * d,
            (m[0][2] + m[2][0]) * d,
            (m[2][1] - m[1][2]) * d
        );
    }
    else if (selected == 1) {
        float y = std::sqrt(py) * 0.5f;
        float d = 1 / (4 * y);
        return Quaternion(
            (m[1][0] + m[0][1]) * d,
            y,
            (m[2][1] + m[1][2]) * d,
            (m[0][2] - m[2][0]) * d
        );
    }
    else if (selected == 2) {
        float z = std::sqrt(pz) * 0.5f;
        float d = 1 / (4 * z);
        return Quaternion(
            (m[0][2] + m[2][0]) * d,
            (m[2][1] + m[1][2]) * d,
            z,
            (m[1][0] - m[0][1]) * d
        );
    }
    else if (selected == 3) {
        float w = std::sqrt(pw) * 0.5f;
        float d = 1 / (4 * w);
        return Quaternion(
            (m[2][1] - m[1][2]) * d,
            (m[0][2] - m[2][0]) * d,
            (m[1][0] - m[0][1]) * d,
            w
        );
    }
    return Quaternion::IdentityQuaternion();
}

Matrix4x4 Matrix4x4::Transpose(const Matrix4x4& input)
{
    Matrix4x4 tmpm;
    for (int l = 0; l < 4; l++)
    {
        for (int c = 0; c < 4; c++)
        {
            tmpm.m[l][c] = input.m[c][l];
        }
    }
    return tmpm;
}
Matrix4x4 Matrix4x4::Transpose()
{
    Matrix4x4 tmpm;
    for (int l = 0; l < 4; l++)
    {
        for (int c = 0; c < 4; c++)
        {
            tmpm.m[l][c] = this->m[c][l];
        }
    }
    return tmpm;
}

float Matrix4x4::det(const Matrix4x4& input)
{
    float result;
    result = input.m[0][0] * input.m[1][1] * input.m[2][2] * input.m[3][3]
        + input.m[0][0] * input.m[1][2] * input.m[2][3] * input.m[3][1]
        + input.m[0][0] * input.m[1][3] * input.m[2][1] * input.m[3][2]  //3行目
        - input.m[0][0] * input.m[1][3] * input.m[2][2] * input.m[3][1]
        - input.m[0][0] * input.m[1][2] * input.m[2][1] * input.m[3][3]
        - input.m[0][0] * input.m[1][1] * input.m[2][3] * input.m[3][2]  //6行目
        - input.m[0][1] * input.m[1][0] * input.m[2][2] * input.m[3][3]
        - input.m[0][2] * input.m[1][0] * input.m[2][3] * input.m[3][1]
        - input.m[0][3] * input.m[1][0] * input.m[2][1] * input.m[3][2]  //9行目
        + input.m[0][3] * input.m[1][0] * input.m[2][2] * input.m[3][1]
        + input.m[0][2] * input.m[1][0] * input.m[2][1] * input.m[3][3]
        + input.m[0][1] * input.m[1][0] * input.m[2][3] * input.m[3][2]  //12行目
        + input.m[0][1] * input.m[1][2] * input.m[2][0] * input.m[3][3]
        + input.m[0][2] * input.m[1][3] * input.m[2][0] * input.m[3][1]
        + input.m[0][3] * input.m[1][1] * input.m[2][0] * input.m[3][2]  //15行目
        - input.m[0][3] * input.m[1][2] * input.m[2][0] * input.m[3][1]
        - input.m[0][2] * input.m[1][1] * input.m[2][0] * input.m[3][3]
        - input.m[0][1] * input.m[1][3] * input.m[2][0] * input.m[3][2]  //18行目
        - input.m[0][1] * input.m[1][2] * input.m[2][3] * input.m[3][0]
        - input.m[0][2] * input.m[1][3] * input.m[2][1] * input.m[3][0]
        - input.m[0][3] * input.m[1][1] * input.m[2][2] * input.m[3][0]  //21行目
        + input.m[0][3] * input.m[1][2] * input.m[2][1] * input.m[3][0]
        + input.m[0][2] * input.m[1][1] * input.m[2][3] * input.m[3][0]
        + input.m[0][1] * input.m[1][3] * input.m[2][2] * input.m[3][0]; //24行目

        return result;
}
float Matrix4x4::det()
{
    float result;
    result = this->m[0][0] * this->m[1][1] * this->m[2][2] * this->m[3][3]
        + this->m[0][0] * this->m[1][2] * this->m[2][3] * this->m[3][1]
        + this->m[0][0] * this->m[1][3] * this->m[2][1] * this->m[3][2]  //3行目
        - this->m[0][0] * this->m[1][3] * this->m[2][2] * this->m[3][1]
        - this->m[0][0] * this->m[1][2] * this->m[2][1] * this->m[3][3]
        - this->m[0][0] * this->m[1][1] * this->m[2][3] * this->m[3][2]  //6行目
        - this->m[0][1] * this->m[1][0] * this->m[2][2] * this->m[3][3]
        - this->m[0][2] * this->m[1][0] * this->m[2][3] * this->m[3][1]
        - this->m[0][3] * this->m[1][0] * this->m[2][1] * this->m[3][2]  //9行目
        + this->m[0][3] * this->m[1][0] * this->m[2][2] * this->m[3][1]
        + this->m[0][2] * this->m[1][0] * this->m[2][1] * this->m[3][3]
        + this->m[0][1] * this->m[1][0] * this->m[2][3] * this->m[3][2]  //12行目
        + this->m[0][1] * this->m[1][2] * this->m[2][0] * this->m[3][3]
        + this->m[0][2] * this->m[1][3] * this->m[2][0] * this->m[3][1]
        + this->m[0][3] * this->m[1][1] * this->m[2][0] * this->m[3][2]  //15行目
        - this->m[0][3] * this->m[1][2] * this->m[2][0] * this->m[3][1]
        - this->m[0][2] * this->m[1][1] * this->m[2][0] * this->m[3][3]
        - this->m[0][1] * this->m[1][3] * this->m[2][0] * this->m[3][2]  //18行目
        - this->m[0][1] * this->m[1][2] * this->m[2][3] * this->m[3][0]
        - this->m[0][2] * this->m[1][3] * this->m[2][1] * this->m[3][0]
        - this->m[0][3] * this->m[1][1] * this->m[2][2] * this->m[3][0]  //21行目
        + this->m[0][3] * this->m[1][2] * this->m[2][1] * this->m[3][0]
        + this->m[0][2] * this->m[1][1] * this->m[2][3] * this->m[3][0]
        + this->m[0][1] * this->m[1][3] * this->m[2][2] * this->m[3][0]; //24行目

    return result;
}

Matrix4x4 Matrix4x4::Inverse(const Matrix4x4& input)
{
    Matrix4x4 result;
    float resultDet = Matrix4x4::det(input);
    result.m[0][0] = (input.m[1][1] * input.m[2][2] * input.m[3][3] + input.m[1][2] * input.m[2][3] * input.m[3][1] + input.m[1][3] * input.m[2][1] * input.m[3][2]
        - input.m[1][3] * input.m[2][2] * input.m[3][1] - input.m[1][2] * input.m[2][1] * input.m[3][3] - input.m[1][1] * input.m[2][3] * input.m[3][2]) / resultDet;
    result.m[0][1] = (-input.m[0][1] * input.m[2][2] * input.m[3][3] - input.m[0][2] * input.m[2][3] * input.m[3][1] - input.m[0][3] * input.m[2][1] * input.m[3][2]
        + input.m[0][3] * input.m[2][2] * input.m[3][1] + input.m[0][2] * input.m[2][1] * input.m[3][3] + input.m[0][1] * input.m[2][3] * input.m[3][2]) / resultDet;
    result.m[0][2] = (input.m[0][1] * input.m[1][2] * input.m[3][3] + input.m[0][2] * input.m[1][3] * input.m[3][1] + input.m[0][3] * input.m[1][1] * input.m[3][2]
        - input.m[0][3] * input.m[1][2] * input.m[3][1] - input.m[0][2] * input.m[1][1] * input.m[3][3] - input.m[0][1] * input.m[1][3] * input.m[3][2]) / resultDet;
    result.m[0][3] = (-input.m[0][1] * input.m[1][2] * input.m[2][3] - input.m[0][2] * input.m[1][3] * input.m[2][1] - input.m[0][3] * input.m[1][1] * input.m[2][2]
        + input.m[0][3] * input.m[1][2] * input.m[2][1] + input.m[0][2] * input.m[1][1] * input.m[2][3] + input.m[0][1] * input.m[1][3] * input.m[2][2]) / resultDet;

    result.m[1][0] = (-input.m[1][0] * input.m[2][2] * input.m[3][3] - input.m[1][2] * input.m[2][3] * input.m[3][0] - input.m[1][3] * input.m[2][0] * input.m[3][2]
        + input.m[1][3] * input.m[2][2] * input.m[3][0] + input.m[1][2] * input.m[2][0] * input.m[3][3] + input.m[1][0] * input.m[2][3] * input.m[3][2]) / resultDet;
    result.m[1][1] = (input.m[0][0] * input.m[2][2] * input.m[3][3] + input.m[0][2] * input.m[2][3] * input.m[3][0] + input.m[0][3] * input.m[2][0] * input.m[3][2]
        - input.m[0][3] * input.m[2][2] * input.m[3][0] - input.m[0][2] * input.m[2][0] * input.m[3][3] - input.m[0][0] * input.m[2][3] * input.m[3][2]) / resultDet;
    result.m[1][2] = (-input.m[0][0] * input.m[1][2] * input.m[3][3] - input.m[0][2] * input.m[1][3] * input.m[3][0] - input.m[0][3] * input.m[1][0] * input.m[3][2]
        + input.m[0][3] * input.m[1][2] * input.m[3][0] + input.m[0][2] * input.m[1][0] * input.m[3][3] + input.m[0][0] * input.m[1][3] * input.m[3][2]) / resultDet;
    result.m[1][3] = (input.m[0][0] * input.m[1][2] * input.m[2][3] + input.m[0][2] * input.m[1][3] * input.m[2][0] + input.m[0][3] * input.m[1][0] * input.m[2][2]
        - input.m[0][3] * input.m[1][2] * input.m[2][0] - input.m[0][2] * input.m[1][0] * input.m[2][3] - input.m[0][0] * input.m[1][3] * input.m[2][2]) / resultDet;

    result.m[2][0] = (input.m[1][0] * input.m[2][1] * input.m[3][3] + input.m[1][1] * input.m[2][3] * input.m[3][0] + input.m[1][3] * input.m[2][0] * input.m[3][1]
        - input.m[1][3] * input.m[2][1] * input.m[3][0] - input.m[1][1] * input.m[2][0] * input.m[3][3] - input.m[1][0] * input.m[2][3] * input.m[3][1]) / resultDet;
    result.m[2][1] = (-input.m[0][0] * input.m[2][1] * input.m[3][3] - input.m[0][1] * input.m[2][3] * input.m[3][0] - input.m[0][3] * input.m[2][0] * input.m[3][1]
        + input.m[0][3] * input.m[2][1] * input.m[3][0] + input.m[0][1] * input.m[2][0] * input.m[3][3] + input.m[0][0] * input.m[2][3] * input.m[3][1]) / resultDet;
    result.m[2][2] = (input.m[0][0] * input.m[1][1] * input.m[3][3] + input.m[0][1] * input.m[1][3] * input.m[3][0] + input.m[0][3] * input.m[1][0] * input.m[3][1]
        - input.m[0][3] * input.m[1][1] * input.m[3][0] - input.m[0][1] * input.m[1][0] * input.m[3][3] - input.m[0][0] * input.m[1][3] * input.m[3][1]) / resultDet;
    result.m[2][3] = (-input.m[0][0] * input.m[1][1] * input.m[2][3] - input.m[0][1] * input.m[1][3] * input.m[2][0] - input.m[0][3] * input.m[1][0] * input.m[2][1]
        + input.m[0][3] * input.m[1][1] * input.m[2][0] + input.m[0][1] * input.m[1][0] * input.m[2][3] + input.m[0][0] * input.m[1][3] * input.m[2][1]) / resultDet;

    result.m[3][0] = (-input.m[1][0] * input.m[2][1] * input.m[3][2] - input.m[1][1] * input.m[2][2] * input.m[3][0] - input.m[1][2] * input.m[2][0] * input.m[3][1]
        + input.m[1][2] * input.m[2][1] * input.m[3][0] + input.m[1][1] * input.m[2][0] * input.m[3][2] + input.m[1][0] * input.m[2][2] * input.m[3][1]) / resultDet;
    result.m[3][1] = (input.m[0][0] * input.m[2][1] * input.m[3][2] + input.m[0][1] * input.m[2][2] * input.m[3][0] + input.m[0][2] * input.m[2][0] * input.m[3][1]
        - input.m[0][2] * input.m[2][1] * input.m[3][0] - input.m[0][1] * input.m[2][0] * input.m[3][2] - input.m[0][0] * input.m[2][2] * input.m[3][1]) / resultDet;
    result.m[3][2] = (-input.m[0][0] * input.m[1][1] * input.m[3][2] - input.m[0][1] * input.m[1][2] * input.m[3][0] - input.m[0][2] * input.m[1][0] * input.m[3][1]
        + input.m[0][2] * input.m[1][1] * input.m[3][0] + input.m[0][1] * input.m[1][0] * input.m[3][2] + input.m[0][0] * input.m[1][2] * input.m[3][1]) / resultDet;
    result.m[3][3] = (input.m[0][0] * input.m[1][1] * input.m[2][2] + input.m[0][1] * input.m[1][2] * input.m[2][0] + input.m[0][2] * input.m[1][0] * input.m[2][1]
        - input.m[0][2] * input.m[1][1] * input.m[2][0] - input.m[0][1] * input.m[1][0] * input.m[2][2] - input.m[0][0] * input.m[1][2] * input.m[2][1]) / resultDet;
    return result;
}
Matrix4x4 Matrix4x4::Inverse()
{
    Matrix4x4 result;
    float resultDet = Matrix4x4::det(*this);
    result.m[0][0] = (this->m[1][1] * this->m[2][2] * this->m[3][3] + this->m[1][2] * this->m[2][3] * this->m[3][1] + this->m[1][3] * this->m[2][1] * this->m[3][2]
        - this->m[1][3] * this->m[2][2] * this->m[3][1] - this->m[1][2] * this->m[2][1] * this->m[3][3] - this->m[1][1] * this->m[2][3] * this->m[3][2]) / resultDet;
    result.m[0][1] = (-this->m[0][1] * this->m[2][2] * this->m[3][3] - this->m[0][2] * this->m[2][3] * this->m[3][1] - this->m[0][3] * this->m[2][1] * this->m[3][2]
        + this->m[0][3] * this->m[2][2] * this->m[3][1] + this->m[0][2] * this->m[2][1] * this->m[3][3] + this->m[0][1] * this->m[2][3] * this->m[3][2]) / resultDet;
    result.m[0][2] = (this->m[0][1] * this->m[1][2] * this->m[3][3] + this->m[0][2] * this->m[1][3] * this->m[3][1] + this->m[0][3] * this->m[1][1] * this->m[3][2]
        - this->m[0][3] * this->m[1][2] * this->m[3][1] - this->m[0][2] * this->m[1][1] * this->m[3][3] - this->m[0][1] * this->m[1][3] * this->m[3][2]) / resultDet;
    result.m[0][3] = (-this->m[0][1] * this->m[1][2] * this->m[2][3] - this->m[0][2] * this->m[1][3] * this->m[2][1] - this->m[0][3] * this->m[1][1] * this->m[2][2]
        + this->m[0][3] * this->m[1][2] * this->m[2][1] + this->m[0][2] * this->m[1][1] * this->m[2][3] + this->m[0][1] * this->m[1][3] * this->m[2][2]) / resultDet;

    result.m[1][0] = (-this->m[1][0] * this->m[2][2] * this->m[3][3] - this->m[1][2] * this->m[2][3] * this->m[3][0] - this->m[1][3] * this->m[2][0] * this->m[3][2]
        + this->m[1][3] * this->m[2][2] * this->m[3][0] + this->m[1][2] * this->m[2][0] * this->m[3][3] + this->m[1][0] * this->m[2][3] * this->m[3][2]) / resultDet;
    result.m[1][1] = (this->m[0][0] * this->m[2][2] * this->m[3][3] + this->m[0][2] * this->m[2][3] * this->m[3][0] + this->m[0][3] * this->m[2][0] * this->m[3][2]
        - this->m[0][3] * this->m[2][2] * this->m[3][0] - this->m[0][2] * this->m[2][0] * this->m[3][3] - this->m[0][0] * this->m[2][3] * this->m[3][2]) / resultDet;
    result.m[1][2] = (-this->m[0][0] * this->m[1][2] * this->m[3][3] - this->m[0][2] * this->m[1][3] * this->m[3][0] - this->m[0][3] * this->m[1][0] * this->m[3][2]
        + this->m[0][3] * this->m[1][2] * this->m[3][0] + this->m[0][2] * this->m[1][0] * this->m[3][3] + this->m[0][0] * this->m[1][3] * this->m[3][2]) / resultDet;
    result.m[1][3] = (this->m[0][0] * this->m[1][2] * this->m[2][3] + this->m[0][2] * this->m[1][3] * this->m[2][0] + this->m[0][3] * this->m[1][0] * this->m[2][2]
        - this->m[0][3] * this->m[1][2] * this->m[2][0] - this->m[0][2] * this->m[1][0] * this->m[2][3] - this->m[0][0] * this->m[1][3] * this->m[2][2]) / resultDet;

    result.m[2][0] = (this->m[1][0] * this->m[2][1] * this->m[3][3] + this->m[1][1] * this->m[2][3] * this->m[3][0] + this->m[1][3] * this->m[2][0] * this->m[3][1]
        - this->m[1][3] * this->m[2][1] * this->m[3][0] - this->m[1][1] * this->m[2][0] * this->m[3][3] - this->m[1][0] * this->m[2][3] * this->m[3][1]) / resultDet;
    result.m[2][1] = (-this->m[0][0] * this->m[2][1] * this->m[3][3] - this->m[0][1] * this->m[2][3] * this->m[3][0] - this->m[0][3] * this->m[2][0] * this->m[3][1]
        + this->m[0][3] * this->m[2][1] * this->m[3][0] + this->m[0][1] * this->m[2][0] * this->m[3][3] + this->m[0][0] * this->m[2][3] * this->m[3][1]) / resultDet;
    result.m[2][2] = (this->m[0][0] * this->m[1][1] * this->m[3][3] + this->m[0][1] * this->m[1][3] * this->m[3][0] + this->m[0][3] * this->m[1][0] * this->m[3][1]
        - this->m[0][3] * this->m[1][1] * this->m[3][0] - this->m[0][1] * this->m[1][0] * this->m[3][3] - this->m[0][0] * this->m[1][3] * this->m[3][1]) / resultDet;
    result.m[2][3] = (-this->m[0][0] * this->m[1][1] * this->m[2][3] - this->m[0][1] * this->m[1][3] * this->m[2][0] - this->m[0][3] * this->m[1][0] * this->m[2][1]
        + this->m[0][3] * this->m[1][1] * this->m[2][0] + this->m[0][1] * this->m[1][0] * this->m[2][3] + this->m[0][0] * this->m[1][3] * this->m[2][1]) / resultDet;

    result.m[3][0] = (-this->m[1][0] * this->m[2][1] * this->m[3][2] - this->m[1][1] * this->m[2][2] * this->m[3][0] - this->m[1][2] * this->m[2][0] * this->m[3][1]
        + this->m[1][2] * this->m[2][1] * this->m[3][0] + this->m[1][1] * this->m[2][0] * this->m[3][2] + this->m[1][0] * this->m[2][2] * this->m[3][1]) / resultDet;
    result.m[3][1] = (this->m[0][0] * this->m[2][1] * this->m[3][2] + this->m[0][1] * this->m[2][2] * this->m[3][0] + this->m[0][2] * this->m[2][0] * this->m[3][1]
        - this->m[0][2] * this->m[2][1] * this->m[3][0] - this->m[0][1] * this->m[2][0] * this->m[3][2] - this->m[0][0] * this->m[2][2] * this->m[3][1]) / resultDet;
    result.m[3][2] = (-this->m[0][0] * this->m[1][1] * this->m[3][2] - this->m[0][1] * this->m[1][2] * this->m[3][0] - this->m[0][2] * this->m[1][0] * this->m[3][1]
        + this->m[0][2] * this->m[1][1] * this->m[3][0] + this->m[0][1] * this->m[1][0] * this->m[3][2] + this->m[0][0] * this->m[1][2] * this->m[3][1]) / resultDet;
    result.m[3][3] = (this->m[0][0] * this->m[1][1] * this->m[2][2] + this->m[0][1] * this->m[1][2] * this->m[2][0] + this->m[0][2] * this->m[1][0] * this->m[2][1]
        - this->m[0][2] * this->m[1][1] * this->m[2][0] - this->m[0][1] * this->m[1][0] * this->m[2][2] - this->m[0][0] * this->m[1][2] * this->m[2][1]) / resultDet;
    return result;
}