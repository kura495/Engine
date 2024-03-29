#pragma once
#include"Math_Structs.h"
#include<cmath>
#include<cassert>

Vector3 Add(const Vector3& v1, const Vector3& v2);
Vector3 Subtract(const Vector3& v1, const Vector3& v2);
Vector3 Normalize(const Vector3& v1);
Vector3 VectorLerp(const Vector3& v1, const Vector3& v2, float t); 
Vector2 VectorLerp(const Vector2& v1, const Vector2& v2, float t);
Vector3 Cross(const Vector3& v1, const Vector3& v2);
Vector3 VectorTransform(const Vector3& vector, const Matrix4x4& matrix);