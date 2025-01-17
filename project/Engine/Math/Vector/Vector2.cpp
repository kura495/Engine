#include "Vector2.h"
using namespace Math;
Vector2 Vector2::VectorLerp(const Vector2& v1, const Vector2& v2, float t) {
	Vector2 result;
	if (t < 0) {
		t = (float)std::fminf(0, 1);
	}
	else if (t > 1) {
		t = (float)std::fmaxf(0, 1);
	}

	result.x = v1.x + t * (v2.x - v1.x);
	result.y = v1.y + t * (v2.y - v1.y);

	return result;
}
