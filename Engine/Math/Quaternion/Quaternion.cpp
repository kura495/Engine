#include "Quaternion.h"
using namespace Math;
Quaternion Quaternion::IdentityQuaternion()
{
	return Quaternion({0.0f,0.0f,0.0f,1.0f});
}
Quaternion Quaternion::Conjugate(const Quaternion& quaternion)
{
	return Quaternion({ quaternion.x*-1.0f,quaternion.y * -1.0f ,quaternion.z * -1.0f ,quaternion.w});
}
Quaternion Quaternion::Conjugate()
{
	return Quaternion({ this->x * -1.0f,this->y * -1.0f ,this->z * -1.0f ,this->w });
}
float Quaternion::Norm(const Quaternion& quaternion)
{
	return sqrt(quaternion.x * quaternion.x + quaternion.y * quaternion.y + quaternion.z * quaternion.z + quaternion.w * quaternion.w);
}
float Quaternion::Norm()
{
	return sqrt(this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w);
}
Quaternion Quaternion::Inverse(const Quaternion& quaternion)
{
	Quaternion result;
	float norm = Norm(quaternion);
	norm = norm * norm;
	Quaternion conj = Conjugate(quaternion);
	if (norm != 0.0f) {
		result.x = conj.x / norm;
		result.y = conj.y / norm;
		result.z = conj.z / norm;
		result.w = conj.w / norm;
	}
	return result;
}
Quaternion Quaternion::Inverse()
{
	Quaternion result;
	float norm = this->Norm();
	norm = norm * norm;
	Quaternion conj = this->Conjugate();
	if (norm != 0.0f) {
		result.x = conj.x / norm;
		result.y = conj.y / norm;
		result.z = conj.z / norm;
		result.w = conj.w / norm;
	}
	return result;
}
Quaternion Quaternion::Normalize(const Quaternion& quaternion)
{
	Quaternion result{};
	float norm = Norm(quaternion);
	if (norm != 0.0f) {
		result.x = quaternion.x / norm;
		result.y = quaternion.y / norm;
		result.z = quaternion.z / norm;
		result.w = quaternion.w / norm;
	}
	return result;
}
Quaternion Quaternion::Normalize()
{
	Quaternion result{};
	float norm = this->Norm();
	if (norm != 0.0f) {
		result.x = this->x / norm;
		result.y = this->y / norm;
		result.z = this->z / norm;
		result.w = this->w / norm;
	}
	return result;
}
Quaternion Quaternion::Slerp(const Quaternion& q0, const Quaternion& q1, float t)
{
	Quaternion result;
	Quaternion Localq0 = q0;
	Quaternion Localq1 = q1;
	//q0とq1の内積
	float dot = Localq0.x * Localq1.x + Localq0.y * Localq1.y + Localq0.z * Localq1.z + Localq0.w * Localq1.w;
	if (dot < 0.0f) {
		//もう片方の回転を利用
		Localq0 = { -q0.x, -q0.y, -q0.z, -q0.w };
		//内積も反転
		dot = -dot;
	}
	if (dot >= 1.0f - std::numeric_limits<float>::epsilon()) {
		result.x = (1.0f - t) * Localq0.x + t * Localq1.x;
		result.y = (1.0f - t) * Localq0.y + t * Localq1.y;
		result.z = (1.0f - t) * Localq0.z + t * Localq1.z;
		result.w = (1.0f - t) * Localq0.w + t * Localq1.w;
		return result;
	}
	//なす角を求める
	float theta = std::acos(dot);

	float scale0 = std::sin((1 - t) * theta) / std::sin(theta);
	float scale1 = std::sin(t * theta) / std::sin(theta);
	result.x = scale0 * Localq0.x + scale1 * Localq1.x;
	result.y = scale0 * Localq0.y + scale1 * Localq1.y;
	result.z = scale0 * Localq0.z + scale1 * Localq1.z;
	result.w = scale0 * Localq0.w + scale1 * Localq1.w;

	return result;
}

Quaternion Quaternion::EulerToQuaterion(Vector3 input)
{

	__m128 result4 = DirectX::XMQuaternionRotationRollPitchYawFromVector({ input.x, input.y, input.z });

	// float[4]型の配列を定義
	float arr[4];

	// __m128型からfloat[4]型にデータをコピー
	_mm_storeu_ps(arr, result4);

	Quaternion result;
	result.x = arr[0];
	result.y = arr[1];
	result.z = arr[2];
	result.w = arr[3];
	
	return result;
}

Quaternion Quaternion::MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle)
{
	Quaternion result;
	result.x = axis.x * std::sin(angle/2);
	result.y = axis.y * std::sin(angle/2);
	result.z = axis.z * std::sin(angle / 2);
	result.w = std::cos(angle/2);
	return result;
}

Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion)
{
	Quaternion fromVector = {vector.x,vector.y,vector.z,0.0f};
	Quaternion conj = Quaternion::Conjugate(quaternion);
	Quaternion rotate = quaternion * fromVector * conj;
	Vector3 result;
	result.x = rotate.x;
	result.y = rotate.y;
	result.z = rotate.z;

	return result;
}