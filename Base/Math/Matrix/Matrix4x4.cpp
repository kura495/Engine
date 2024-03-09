#include "Matrix4x4.h"

Vector3 Matrix4x4::GetScale() {
	
	return {
		Vector3::Length({m[0][0],m[0][1],m[0][2]}),
		Vector3::Length({m[1][0],m[1][1],m[1][2]}),
		Vector3::Length({m[2][0],m[2][1],m[2][2]})};
}

//Vector3 Matrix4x4::GetRotation(const Matrix4x4& m1) {
//
//	return { 0,0,0 };
//}
