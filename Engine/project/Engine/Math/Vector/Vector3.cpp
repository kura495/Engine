#include "Vector3.h"
using namespace Math;
float Vector3::Dot(const Vector3& input1, const Vector3& input2) {
	float result = (input1.x * input2.x) + (input1.y * input2.y) + (input1.z * input2.z);
	return result;
}
Vector3 Vector3::Cross(const Vector3& input1, const Vector3& vinput2)
{
	Vector3 result;
	result.x = (input1.y * vinput2.z) - (input1.z * vinput2.y);
	result.y = (input1.z * vinput2.x) - (input1.x * vinput2.z);
	result.z = (input1.x * vinput2.y) - (input1.y * vinput2.x);
	return result;
};
float Vector3::Length(const Vector3& input) {
	float result{};
	result = sqrt(Dot({ input.x,input.y,input.z }, { input.x,input.y,input.z }));
	return result;
}
float Vector3::Length() {
	float result{};
	result = sqrt(Dot({ this->x,this->y,this->z }, { this->x,this->y,this->z }));
	return result;
}
float Vector3::Distance(const Vector3& input1, const Vector3& input2)
{
	Vector3 vector = input1 - input2;
	float result = sqrt(Dot(vector, vector));
	return result;
}
Vector3 Vector3::Normalize(const Vector3& input) {
	Vector3 result{};
	float length = sqrt(input.x * input.x + input.y * input.y + input.z * input.z);
	if (length != 0.0f) {
		result.x = input.x / length;
		result.y = input.y / length;
		result.z = input.z / length;
	}
	return result;
}
Vector3 Vector3::Normalize() {
	Vector3 result{};
	float length = sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
	if (length != 0.0f) {
		result.x = this->x / length;
		result.y = this->y / length;
		result.z = this->z / length;
	}
	return result;
}
Vector3 Vector3::Lerp(const Vector3& v1, const Vector3& v2, float t) {
	Vector3 result;
	if (t < 0) {
		t = (float)std::min(0, 1);
	}
	else if (t > 1) {
		t = (float)std::max(0, 1);
	}

	result.x = v1.x + t * (v2.x - v1.x);
	result.y = v1.y + t * (v2.y - v1.y);
	result.z = v1.z + t * (v2.z - v1.z);

	return result;
}