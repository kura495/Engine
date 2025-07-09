#pragma once
#include "../InterFace/Iwork.h"
#include <random>
#include <numbers>

class FOV : public Iwork {
public:
	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="FOVvalue">視野角の値</param>
	void Init(float FOVvalue) {
		easeT = 0.0f;
		flag = true;
		newFOVvalue = FOVvalue;
	}
	/// <summary>
	/// FOVを線形補間しながら変更
	/// </summary>
	/// <param name="FOV">変更するFOVの値</param>
	void Update(float& FOV) {
		if (flag) {
			easeT = (std::min)(easeT + AddEaseT, 1.0f);
			FOV = Lerp(FOV, newFOVvalue, easeT);
			if (easeT == 1.0f) {
				flag = false;
			}
		}
	}
private:
	//変更する値
	float newFOVvalue = 0.0f;
};