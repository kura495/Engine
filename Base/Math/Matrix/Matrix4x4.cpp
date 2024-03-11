#include "Matrix4x4.h"

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
    return IdentityQuaternion();
}
