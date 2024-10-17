#pragma once
#include "Math_Structs.h"
#include "externals/nlohmann/json.hpp"
#include <iostream>

struct Transform {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};
class TransformQua final {
public:
	Vector3 scale;
	Quaternion quaternion;
	Vector3 translate;

	//jsonにマクロを利用して構造体を登録
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(TransformQua,scale,quaternion,translate)
public:
	TransformQua operator+(const TransformQua& t2) {
		TransformQua result{};
		result.scale = scale + t2.scale;
		result.quaternion = quaternion + t2.quaternion;
		result.translate = translate + t2.translate;
		return result;
	}
	TransformQua operator+=(const TransformQua& t2) {
		TransformQua result{
			.scale = scale + t2.scale,
			.quaternion = quaternion + t2.quaternion,
			.translate = translate + t2.translate
		};
		return result;
	}
	TransformQua operator=(const TransformQua& t1) {
		
		this->scale = t1.scale;
		this->quaternion = t1.quaternion;
		this->translate = t1.translate;

		return *this;
	}
};