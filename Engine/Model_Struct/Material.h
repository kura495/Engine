#pragma once
#include "Math_Structs.h"
#include <stdint.h>
#include <string>
using namespace Math;
struct Material {
	Vector4 color;
	int32_t enableLighting;
	float padding[3];
	Matrix4x4 uvTransform;
	float shiniess;//光沢度
};
struct MaterialData {
	std::string textureFilePath;
};
