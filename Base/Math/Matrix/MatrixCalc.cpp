#include "MatrixCalc.h"
#include <limits>

Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
	Matrix4x4 result = {
		scale.x, 0.0f, 0.0f, 0.0f,
		0.0f, scale.y, 0.0f, 0.0f,
		0.0f, 0.0f, scale.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};
	return result;
}
Matrix4x4 MakeRotateXMatrix(float radian) {
	Matrix4x4 result = {
		1.0f,0.0f,0.0f,0.0f,
		0.0f,std::cos(radian),std::sin(radian),0.0f,
		0.0f,-std::sin(radian),std::cos(radian),0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
	return result;
}
Matrix4x4 MakeRotateYMatrix(float radian) {
	Matrix4x4 result = {
		std::cos(radian), 0.0f, -std::sin(radian), 0.0f,
		0.0f,1.0f, 0.0f, 0.0f,
		std::sin(radian), 0.0f, std::cos(radian),  0.0f,
		0.0f, 0.0f, 0.0f, 1.0f };
	return result;
}
Matrix4x4 MakeRotateZMatrix(float radian) {
	Matrix4x4 result = {
		std::cos(radian),std::sin(radian),0.0f,0.0f,
		-std::sin(radian),std::cos(radian),0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f };
	return result;
}
Matrix4x4 MakeRotateMatrix(Vector3 rotation)
{
	Matrix4x4 result;
	result = Matrix4x4::CreateIdentity();
	result = Matrix4x4::Multiply(MakeRotateXMatrix(rotation.x), Matrix4x4::Multiply(MakeRotateYMatrix(rotation.y),MakeRotateZMatrix(rotation.z)));
	return result;
}
Matrix4x4 MakeRotateMatrix(const Quaternion& quaternion)
{
	Matrix4x4 result;
	
	result.m[0][0] = (quaternion.w * quaternion.w) + (quaternion.x * quaternion.x) - (quaternion.y * quaternion.y) - (quaternion.z * quaternion.z);
	result.m[0][1] = 2.0f * ((quaternion.x * quaternion.y) + (quaternion.w * quaternion.z));
	result.m[0][2] = 2.0f * ((quaternion.x * quaternion.z) - (quaternion.w * quaternion.y));
	result.m[0][3] = 0.0f;

	result.m[1][0] = 2.0f * ((quaternion.x * quaternion.y) - (quaternion.w * quaternion.z));
	result.m[1][1] = (quaternion.w * quaternion.w) - (quaternion.x * quaternion.x) + (quaternion.y * quaternion.y) - (quaternion.z * quaternion.z);
	result.m[1][2] = 2.0f * ((quaternion.y * quaternion.z) + (quaternion.w * quaternion.x));
	result.m[1][3] = 0.0f;

	result.m[2][0] = 2.0f * ((quaternion.x * quaternion.z) + (quaternion.w * quaternion.y));
	result.m[2][1] = 2.0f * ((quaternion.y * quaternion.z) - (quaternion.w * quaternion.x));
	result.m[2][2] = (quaternion.w * quaternion.w) - (quaternion.x * quaternion.x) - (quaternion.y * quaternion.y) + (quaternion.z * quaternion.z);
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;
}
Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
	Matrix4x4 result = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		translate.x, translate.y, translate.z, 1.0f,
	};
	return result;
}
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	Matrix4x4 Scaleresult = MakeScaleMatrix(scale);
	Matrix4x4 Rotateresult = MakeRotateMatrix(rotate);
	Matrix4x4 Transformresult = MakeTranslateMatrix(translate);
	Matrix4x4 result = Matrix4x4::Multiply(Scaleresult, Matrix4x4::Multiply(Rotateresult, Transformresult));

	return result;
}
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Quaternion& rotate, const Vector3& translate) {
	Matrix4x4 Scaleresult = MakeScaleMatrix(scale);
	Matrix4x4 Rotateresult = MakeRotateMatrix(rotate);
	Matrix4x4 Transformresult = MakeTranslateMatrix(translate);
	Matrix4x4 result = Matrix4x4::Multiply(Scaleresult, Matrix4x4::Multiply(Rotateresult, Transformresult));

	return result;
}

Matrix4x4 MakePerspectiveFovMatrix(float FovY, float aspectRatio, float nearClip, float farClip)
{
	FovY = FovY * ((float)std::numbers::pi / 180);

	Matrix4x4 result = {
	cot(FovY / 2) / aspectRatio,0,0,0,
	0,cot(FovY / 2),0,0,
	0,0,farClip / (farClip - nearClip),1,
	0,0,(-nearClip * farClip) / (farClip - nearClip),0
	};
	return result;
}
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth)
{
	Matrix4x4 result = {
	width / 2,0,0,0,
	0,-(height / 2),0,0,
	0,0,maxDepth - minDepth,0,
	left + (width / 2),top + (height / 2),minDepth,1
	};
	return result;
}
float cot(float top)
{
	float result = 1 / tan(top);
	return result;
}
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farCcip)
{
	Matrix4x4 result;
	float dx = right - left;
	float dy = top - bottom;
	float dz = farCcip - nearClip;
	result = {
		2.0f / dx,0.0f,0.0f,0.0f,
		0.0f,2.0f / dy,0.0f,0.0f,
		0.0f,0.0f,-2.0f / dz,0.0f,
		-((right + left) / dx),-((top + bottom) / dy),-((farCcip + nearClip) / dz),1.0f
	};
	return result;
}
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m) {
	return Vector3(
		v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0],
		v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1],
		v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2]
	);
}
Vector3 GetXAxis(Matrix4x4 matrix)
{
	return Vector3(matrix.m[0][0], matrix.m[0][1], matrix.m[0][2]);
}
Vector3 GetYAxis(Matrix4x4 matrix)
{
	return Vector3(matrix.m[1][0], matrix.m[1][1], matrix.m[1][2]);
}
Vector3 GetZAxis(Matrix4x4 matrix)
{
	return Vector3(matrix.m[2][0], matrix.m[2][1], matrix.m[2][2]);
}
Matrix4x4 DirectionToDirection(const Vector3& from, const Vector3& to)
{
	Matrix4x4 result;
	Vector3 cross = Vector3::Cross(from, to);
	Vector3 n = Vector3::Normalize(Vector3::Cross(from, to));
	// u = -v のとき　つまり反転してしまった時
	if (from.x == -to.x && from.y == -to.y && from.z == -to.z) {
		if (from.x != 0.0f || from.y != 0.0f) {
			n = { from.y,-from.x,0.0f };
		}
		else if (from.x != 0.0f || from.z != 0.0f) {
			n = { from.z,0.0f,-from.x };
		}
	}

	float costhata = Vector3::Dot(from,to);
	float sinthata = cross.Length();
	result.m[0][0] = (n.x * n.x) * (1 - costhata) + costhata;
	result.m[0][1] = (n.x * n.y) * (1 - costhata) + n.z * sinthata;
	result.m[0][2] = (n.x * n.z) * (1 - costhata) - n.y * sinthata;
	result.m[0][3] = 0;

	result.m[1][0] = (n.x * n.y) * (1 - costhata) - n.z * sinthata;
	result.m[1][1] = (n.y * n.y) * (1 - costhata) + costhata;
	result.m[1][2] = (n.y * n.z) * (1 - costhata) + n.x * sinthata;
	result.m[1][3] = 0;

	result.m[2][0] = (n.x * n.z) * (1 - costhata) + n.y * sinthata;
	result.m[2][1] = (n.y * n.z) * (1 - costhata) - n.x * sinthata;
	result.m[2][2] = (n.z * n.z) * (1 - costhata) + costhata;
	result.m[2][3] = 0;

	result.m[3][0] = 0;
	result.m[3][1] = 0;
	result.m[3][2] = 0;
	result.m[3][3] = 1;

	return result;
}
Matrix4x4 MakeRotateAxisAngle(const Vector3& axis, float angle)
{
	Matrix4x4 result;

	result.m[0][0] = (axis.x * axis.x) * (1 - cos(angle)) + cos(angle);
	result.m[0][1] = (axis.x * axis.y) * (1 - cos(angle)) + axis.z * sin(angle);
	result.m[0][2] = (axis.x * axis.z) * (1 - cos(angle)) - axis.y * sin(angle);
	result.m[0][3] = 0;

	result.m[1][0] = (axis.x * axis.y) * (1 - cos(angle)) - axis.z * sin(angle);
	result.m[1][1] = (axis.y * axis.y) * (1 - cos(angle)) + cos(angle);
	result.m[1][2] = (axis.y * axis.z) * (1 - cos(angle)) + axis.x * sin(angle);
	result.m[1][3] = 0;

	result.m[2][0] = (axis.x * axis.z) * (1 - cos(angle)) + axis.y * sin(angle);
	result.m[2][1] = (axis.y * axis.z) * (1 - cos(angle)) - axis.x * sin(angle);
	result.m[2][2] = (axis.z * axis.z) * (1 - cos(angle)) + cos(angle);
	result.m[2][3] = 0;

	result.m[3][0] = 0;
	result.m[3][1] = 0;
	result.m[3][2] = 0;
	result.m[3][3] = 1;

	return result;
}


