#pragma once
#include "PostProsess/renderTextrue/PostProsess.h"

class RGBshift : public PostProsess
{
public:
	void Update()override;
	static bool isEnableFlag;
private:


	//現在のTの値
	float easeT = 0.0f;
	//raseTに毎フレーム加算する値
	float addEaseT = 0.0005f;

	const float kEaseTMax = 0.04f;
};

