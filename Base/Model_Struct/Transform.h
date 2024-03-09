#pragma once
#include "Math_Structs.h"
#include "externals/nlohmann/json.hpp"
#include <iostream>

struct Transform {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};
class TransformQua {
public:
	Vector3 scale;
	Quaternion quaternion;
	Vector3 translate;
	//jsonにマクロを利用して構造体を登録
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(TransformQua,scale,quaternion,translate)
public:
	friend inline TransformQua operator+(const TransformQua& t1, const TransformQua& t2) {
		TransformQua result;
		result.scale = t1.scale + t2.scale;
		result.quaternion = t1.quaternion + t2.quaternion;
		result.translate = t1.translate + t2.translate;
		return result;
	}
	friend inline TransformQua operator+=(const TransformQua& t1, const TransformQua& t2) {
		TransformQua result{
		
		};
		result.scale = t1.scale + t2.scale;
		result.quaternion = t1.quaternion + t2.quaternion;
		result.translate = t1.translate + t2.translate;
		return result;
	}	
	inline TransformQua operator=(const TransformQua& t1) {
		TransformQua result{
			.scale = t1.scale,
			.quaternion = t1.quaternion,
			.translate = t1.translate
		};
		return result;
	}	
};