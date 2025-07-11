#pragma once
#include "../InterFace/Iwork.h"
#include <random>
#include <numbers>

class WorkFOV : public Iwork {
public:
	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="FOVvalue">視野角の値</param>
	void Init(float FOVvalue);
	/// <summary>
	/// FOVを線形補間しながら変更
	/// </summary>
	/// <param name="FOV">変更するFOVの値</param>
	void Update(float& FOV);
private:
	//変更する値
	float newFOVvalue = 0.0f;
};